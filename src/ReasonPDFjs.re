open Webapi;

let initPDFViewer = (pdfURL, container) => {
  let documentPromise = BsPdfjs.Global.inst |> BsPdfjs.Global.getDocument(pdfURL);
  documentPromise
  |> Js.Promise.then_(document => {
       let pagePromise = document |> BsPdfjs.Document.getPage(1);
       pagePromise
       |> Js.Promise.then_(page => {
            let pdfViewport: BsPdfjs.Viewport.t =
              BsPdfjs.Page.getViewport(
                ~scale=
                  container##offsetWidth
                  /. BsPdfjs.Viewport.width(BsPdfjs.Page.getViewport(~scale=1., ~rotate=0., page)),
                ~rotate=0.,
                page,
              );
            let canvas =
              switch (Dom.document |> Dom.Document.querySelector("canvas")) {
              | None =>
                print_endline("cant get canvas\n");
                failwith("fail");
              | Some(el) => el
              };
            let context = CanvasRe.CanvasElement.getContext2d(canvas);
            canvas |> Dom.Element.setAttribute("width", string_of_float(BsPdfjs.Viewport.width(pdfViewport)));
            canvas |> Dom.Element.setAttribute("height", string_of_float(BsPdfjs.Viewport.height(pdfViewport)));
            BsPdfjs.Page.render(page, ~canvasContext=context, ~viewport=pdfViewport, ~transform=Js.Option.some([||]))
            |> ignore;
            Js.Promise.resolve(page);
          })
       |> ignore;
       Js.Promise.resolve(document);
     });
};
