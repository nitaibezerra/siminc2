
// Essa funcao estÃ¡ aqui porque nÃ£o precisa ser duplicada uma vez que
// faz a mesma coisa em todos os lugares em que for chamada
function abreListaSupervisaoFnde( obrid, empid ){
	// window.location.href = 'obras2.php?modulo=principal/listaSupervisaoFNDE&acao=A&obrid='+obrid;
	$('[name=req]').val( 'supervisorFNDE' );
	$('[name=obrid]').val( obrid );
	$('[name=empid]').val( empid );


	$('#formListaObra').submit();
}


function validarPercentual( valor ){
    var inicio = new Number( document.getElementById( 'percentualinicial' ).value );
    var fim    = new Number( document.getElementById( 'percentualfinal' ).value );

    if ( inicio > fim ){
        alert('O valor percentual mínimo é maior do que o máximo');
        if ( fim > 5 ){
            document.getElementById( 'percentualinicial' ).value = fim - 5;
        }else{
            document.getElementById( 'percentualinicial' ).value = 0;
        }
    }
} 
