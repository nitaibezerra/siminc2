function imprimirCurso(curid){
	return window.open('catalogocurso.php?modulo=principal/impressaoCurso&acao=A&curid='+curid, 
					   'modelo', 
					   "height=600,width=950,scrollbars=yes,top=50,left=200" );
}