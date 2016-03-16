<?php

if($_REQUEST["enbid"]) {
	$_SESSION["emenda"]["enbid"] = $_REQUEST["enbid"];
	redirecionaUsuario();
}

function redirecionaUsuario() {
	if($_REQUEST["retorno"]) {
		echo '<script type="text/javascript">
					window.location.href = \'emenda.php?modulo=principal/'.$_REQUEST["retorno"].'&acao=A\';
			  </script>';
		die;
	} else {
		echo '<script type="text/javascript">
					window.location.href = \'emenda.php?modulo=principal/listaPlanoTrabalho&acao=A\';
			  </script>';
		die;
	}
}

if($_REQUEST['retorno']){
	$retorno = $_REQUEST['retorno'];
}

$where = "";
$where .= ($_REQUEST["enbcnpj"]) ? " AND e.enbcnpj = '".str_replace( array(".","/","-"), "", $_REQUEST["enbcnpj"] )."'" : "";
$where .= ($_REQUEST["enbnome"]) ? " AND  UPPER( removeacento(e.enbnome) ) ilike '%".( removeAcentos( str_to_upper( trim($_REQUEST["enbnome"]) ) ) )."%'" : "";

$usucpf = $_SESSION['usucpf'];
$pflcod = pegaPerfil($usucpf);

$listarEntidades = false;

if( ($pflcod == INSTITUICAO_BENEFICIADA) ){
	$sql = "SELECT ur.enbid
			FROM 
			  emenda.usuarioresponsabilidade ur
			  inner join emenda.entidadebeneficiada eb on eb.enbid = ur.enbid
			WHERE 
			  ur.rpustatus='A'
			  and eb.enbstatus = 'A'
			  AND ur.enbid is not null
			  AND eb.enbano = '{$_SESSION['exercicio']}'
			  AND ur.usucpf = '$usucpf' AND ur.pflcod = $pflcod";

	$enbid = $db->carregarColuna($sql);
	
	$total = count($enbid);
	
	if($total == 0){
		include  APPRAIZ."includes/cabecalho.inc";
		exibeInstituicaoBenefiada();
		print "<br/>";
		
		$db->cria_aba( $abacod_tela, $url, '' );
		
		monta_titulo( 'Lista de Entidade(s) Beneficiada(s)', 'Você deverá selecionar uma Instituição Beneficiada para continuar utilizando o sistema.' );
		
		exibeFiltroPesquisaInstituicao($retorno);
		print "<br/>";
		?>
		<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<thead>
			<tr>
				<td class="title" bgcolor="" width="10%" align="center" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
					<strong>Selecione uma Instituição</strong>
				</td>
				<td class="title" bgcolor="" width="30%" align="center" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
					<strong>CNPJ</strong>
				</td>
				<td class="title" bgcolor="" width="60%" align="center" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
					<strong>Nome</strong>
				</td>
			</tr>
			</thead>
		<tr bgcolor="#ffffff">
			<td colspan="3"><b>Você não está vinculado a nenhuma instituição</b></td>
		</tr>
		</table>
		<?
		
	}else if($total == 1){
		$_SESSION["emenda"]["enbid"] = $enbid[0];
		redirecionaUsuario();	
		$listarEntidades = true;
	}else{
		$listarEntidades = true;
	}
}
else{
	$listarEntidades = true;
}

if($listarEntidades){

	include  APPRAIZ."includes/cabecalho.inc";
	exibeInstituicaoBenefiada();
	print "<br/>";
	
	$db->cria_aba( $abacod_tela, $url, '' );
	
	monta_titulo( 'Lista de Entidade(s) Beneficiada(s)', 'Você deverá selecionar uma Instituição Beneficiada para continuar utilizando o sistema.' );
	
	exibeFiltroPesquisaInstituicao($retorno);
	print "<br/>";
	
	// '||'	 <img src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"habilitacao('|| e.entnumcpfcnpj || ')\" \" border=0 alt=\"Ir\" title=\"Habilitacao\">
	
	$sql = "SELECT DISTINCT
			  ( '<center><img src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"carregaEntidadeEmenda('|| e.enbid || ')\" \" border=0 alt=\"Ir\" title=\"Plano de Trabalho\"></center>' ) as acao, 
			  e.enbcnpj, 
			  e.enbnome
			FROM 
			  emenda.entidadebeneficiada e
			LEFT JOIN
			  emenda.usuarioresponsabilidade ur ON ur.enbid = e.enbid 
			 WHERE
			 	e.enbstatus = 'A'
			 	and e.enbano = '{$_SESSION['exercicio']}'
			 	".$where;
	

	if( ($pflcod == INSTITUICAO_BENEFICIADA) ){
		$sql .= " AND ur.usucpf = '$usucpf' AND ur.pflcod = $pflcod AND ur.rpustatus='A' AND ur.enbid is not null";
	}
	
?>
<html>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<body>
<?php
	
	//monta_titulo( '', 'Lista de Entidade(s) Beneficiada(s)' );
	//if($total > 0){				  
		$cabecalho = array("Selecione uma Instituição", "CNPJ", "Nome");
		
/*		$arMascara = array( 
							array( 'campo'   => 'enbcnpj',
								   'mascara' => '##.###.###/####-##'),
							array( 'campo'   => 'campotipocnpj',
								   'mascara' => '##.###.###/####-##')
						  );

		$db->monta_lista(iconv( "UTF-8", "ISO-8859-1", $sql), $cabecalho, 20, 4, 'N','Center','','form','','', $arMascara);*/
		$db->monta_lista(iconv( "UTF-8", "ISO-8859-1", $sql), $cabecalho, 20, 4, 'N','Center','','form');
	//}
?>

</body>
<script type="text/javascript">
	function carregaEntidadeEmenda(enbid){
		var retorno = document.getElementById("retorno").value;
		window.location.href = 'emenda.php?modulo=principal/listaInstituicoes&acao=A&enbid='+enbid+'&retorno='+retorno;
	}
	
	function limparPesquisa(){
		document.getElementById("formulario").reset();
	}
	
	function submetePesquisa() {
		var form		=	document.getElementById("formulario");
		var btPesquisar	=	document.getElementById("btPesquisar");
		var btLimpar	=	document.getElementById("btLimpar");
		
		btPesquisar.disabled 	= true;
		btLimpar.disabled 		= true;
	
		enbnome	=	document.getElementsByName("enbnome")[0];
		
		if(enbnome.value.length > 0 && enbnome.value.length < 6) {
			alert("Para realizar a busca por nome do município é necessário informar pelo menos 6 caracteres.");
			enbnome.focus();
			btPesquisar.disabled 	= false;
			btLimpar.disabled 		= false;
			return;
		}
		
		form.submit();
	}
</script>
</html>
<?
}
if( !$db->testa_superuser() && empty($where) ) {
	$sql = "select count(a.arqid) as c
		 	from emenda.anexo de
			inner join emenda.planotrabalho pt on pt.ptrid = de.ptrid
			inner join emenda.entidadebeneficiada eb on eb.enbid = pt.enbid
			inner join public.arquivo a on a.arqid=de.arqid 
			-- inner join emenda.emendadetalheentidade ee on ee.enbid = eb.enbid
			inner join emenda.usuarioresponsabilidade ur on ur.enbid = eb.enbid
			inner join seguranca.usuario u ON u.usucpf = ur.usucpf 
			where a.arqid/1000 between 647 and 725 and 
			a.arqid not in(select arqid from public.arquivo_recuperado) and a.sisid=57 and a.usucpf='".$_SESSION['usucpf']."'";
	
	$carq = $db->pegaUm($sql);
}
if($carq > 0 ) {
	$texto = "<center>
				<img src=\"../imagens/alerta_sistema.gif\" />
			    <p><font size=3 color=red><b>Nota do Sistema!</b></font></p>
           	     <p style=\"font-weight:bold\" ><font size=3>Alguns arquivos anexados por você no SIMEC foram corrompidos,<br />
           	     para corrigir o problema você poderá enviá-los novamente.<br />
           	     Clique abaixo para ver a relação e proceder a correção.</p>
           	     <input type=\"button\" value=\"Ver a relação de arquivos\" style=\"cursor:pointer;font:16px Trebuchet Ms,Arial,Tahoma,Verdana,Helvetica,Sans-Serif;height:33px\" onclick=\"window.location='emenda.php?modulo=principal/emenda_arquivo&acao=A'\" />
          	  </center>";
	popupAlertaGeral($texto,"580px","260px");
}
?>