$(".question").on( "click", function() {
    console.log($(this).index())
    $('#answer'+ $(this).index()).toggle('slow')
})

$(".exam-button").on( "mouseover", function() {
    console.log($(this).index())
    $("#subnav"+$(this).index()).toggle()
})

$(".sub-nav").on('click', function() {
    console.log("clicked")
    $(this).toggle()
})
