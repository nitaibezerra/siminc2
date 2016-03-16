<?php

if ( $_REQUEST["carga"] ){
	academico_busca_obras( $_REQUEST["carga"], '100%', 'naoinauguradas', true  );
	die;
}


if($_REQUEST['exec_function']) {
	$_REQUEST['exec_function']($_REQUEST);
}

monta_titulo( "Inserir Obras", "Selecione a obra desejada");

?>
<html>
	<head>
		<title>Inserir Obras</title>		
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script language="JavaScript" src="./geral/js/academico.js"></script>
		<script type="text/javascript" src="../includes/prototype.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script>
			function crtobrasinauguradas(chk,param) {
				if(chk.checked) {
					academico_ajaxatualizar('exec_function=academico_salvarObrasInauguradas&param='+param,'');
					//window.opener.academico_ajaxatualizar('exec_function=carregarlistaobras&buscar=serinaugurada','td_serinaugurada');
					//window.opener.ajaxatualizar('exec_function=carregarlistaobras&buscar=outras','td_outras');
				} else {
					var conf = confirm("A obra deixará de ser inaugurada. Deseja continuar?");
					if(conf) {
						ajaxatualizar('exec_function=academico_removerObrasInauguradas&param='+param,'');
						//window.opener.academico_ajaxatualizar('exec_function=carregarlistaobras&buscar=serinaugurada','td_serinaugurada');
						//window.opener.academico_ajaxatualizar('exec_function=carregarlistaobras&buscar=outras','td_outras');
					}
				}
			}
		</script>
	</head>
	<body>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td align="right" bgcolor="#CCCCCC">
					<input type="button" value="Ok" onclick="window.close();alert('Operação realizada com sucesso!');window.parent.opener.location.reload();">
				</td>
			</tr>
			<tr>
				<td>
					<?php
						
						switch( $_REQUEST['acao1'] ){
							case 'A':
								academico_busca_campus_obras( $_REQUEST['valor'], '', 'inserir_obrasinauguradas' );
							break;
							case 'C':
								academico_busca_obras( $_REQUEST['valor'], '100%', '', true );
							break;
						}
					?>
				</td>
			</tr>
		</table>
	</body>
	<script type="text/javascript">
	
		var params;
	
	//mantendo a arvore expandida
	<?php if( isset($_REQUEST["carga"]) ){ ?>
		abreconteudo('academico.php?modulo=principal/inserir_obrasinauguradas&acao=A&subAcao=gravarCarga&carga=<?=$_REQUEST["carga"]?>&params=' + params, <?=$_REQUEST["carga"]?>);
	<?php } ?>	
	
	function formatarParametros(){
	    //params = Form.serialize($('pesquisar'));
	    
	}
	
	function desabilitarConteudo( id ){
		var url = 'academico.php?modulo=principal/inserir_obrasinauguradas&acao=A&carga='+id;
		if ( document.getElementById('img'+id).name == '-' ) {
			url = url + '&subAcao=retirarCarga';
			var myAjax = new Ajax.Request(
				url,
				{
					method: 'post',
					asynchronous: false
				});
		}
	}
	
	</script>
	
</html>
