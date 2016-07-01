<?php
header("Content-Type: text/html; charset=ISO-8859-1",true);

$_SESSION['academico']['orgid'] = $_REQUEST['acao'] == 'C' ? 1 : 2;
$acao = $_SESSION['academico']['orgid'] == 1 ? 'C' : 'A';

// instancia o objeto do módulo
$academico = new academico();
$academico_lista = new autoriazacaoconcursos();

if ( $_REQUEST["ano"] ){
	$existe_ano = academico_existeano( $_REQUEST["ano"] );
	if ( !$existe_ano ){
		echo "<script>
				alert('O ano informado não existe!');
				history.back(-1);
			  </script>";
		die;
	}	
}

if ( $_REQUEST["carga"] ){
	$academico_lista->listaportariasprov( $_REQUEST["carga"] );
	die;
}

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$_SESSION["academico"]["tprnivel"] 	= ACA_TPORTARIA_CONCURSO;
$orgid 								= $_SESSION['academico']['orgid'];
$ano 								= $_REQUEST['ano'] ?  $_REQUEST['ano'] : '2008';
$_SESSION['academico']['ano'] 		= $ano;
$arqid 	 							= $_REQUEST['arqid'];
$anxid		 						= $_REQUEST['anxid'];
$prtid = $_REQUEST['prtid'];

if(!empty($_REQUEST['evento']) && ($_REQUEST['evento'] != '') ){
	switch($_REQUEST['evento']){		
		case 'E':			
			$sql_D = "
					UPDATE academico.portarias SET prtstatus = 'I' where prtid = ".$prtid;	
			$db->executar($sql_D);
			$db->commit();
			echo "<script>
					alert('Registro removido com sucesso.');
					window.location = '?modulo=principal/listarPortarias&acao=C&orgid=".$orgid."';
				  </script>";
		exit;
		break;	
		case 'P':
			//filtros
			if($_REQUEST['prtnumero']) {				
				$filtroprocesso[] = "upper(p.prtnumero) like upper('%".$_REQUEST['prtnumero']."%')";
			}
			if($_REQUEST['prtid_busca']) {
				$filtroprocesso[] = "p.prtid= ".$_REQUEST['prtid_busca']." ";
			}
			if($_REQUEST['prgid']) {
				$filtroprocesso[] = "pg.prgid= ".$_REQUEST['prgid']." ";
			}	
			if($_REQUEST['prtdtinclusao']) {
				$filtroprocesso[] = "to_char(p.prtdtinclusao, 'DD/MM/YYYY')= '".$_REQUEST['prtdtinclusao']."' ";
			}		
		break;	
	}
}

if(!empty($_REQUEST['evento_arquivo']) && ($_REQUEST['evento_arquivo'] != '')){	
	switch($_REQUEST['evento_arquivo']){		
	
		case 'DA':
			$dados_arq['arqid'] =$_REQUEST['arqid']; 
			DownloadArquivo($dados_arq);
			exit;
		break;
	}
	
}

$filtroprocesso[] = "p.prtstatus = 'A'" ; 
$filtroprocesso[] = "p.prtano = '".$ano."'" ;
$filtroprocesso[] = "o.orgid = ".$orgid."" ;

$menu[0] = array("descricao" => "2008", "link"=> "/academico/academico.php?modulo=principal/listarPortarias&acao={$acao}&ano=2008");
$menu[1] = array("descricao" => "2009", "link"=> "/academico/academico.php?modulo=principal/listarPortarias&acao={$acao}&ano=2009");
$menu[2] = array("descricao" => "2010", "link"=> "/academico/academico.php?modulo=principal/listarPortarias&acao={$acao}&ano=2010");
$menu[3] = array("descricao" => "2011", "link"=> "/academico/academico.php?modulo=principal/listarPortarias&acao={$acao}&ano=2011");
$menu[4] = array("descricao" => "2012", "link"=> "/academico/academico.php?modulo=principal/listarPortarias&acao={$acao}&ano=2012");
$menu[5] = array("descricao" => "2013", "link"=> "/academico/academico.php?modulo=principal/listarPortarias&acao={$acao}&ano=2013");
$menu[6] = array("descricao" => "2014", "link"=> "/academico/academico.php?modulo=principal/listarPortarias&acao={$acao}&ano=2014");
$menu[7] = array("descricao" => "2015", "link"=> "/academico/academico.php?modulo=principal/listarPortarias&acao={$acao}&ano=2015");
$menu[8] = array("descricao" => "2016", "link"=> "/academico/academico.php?modulo=principal/listarPortarias&acao={$acao}&ano=2016");
$menu[9] = array("descricao" => "2017", "link"=> "/academico/academico.php?modulo=principal/listarPortarias&acao={$acao}&ano=2017");
$menu[10] = array("descricao" => "2018", "link"=> "/academico/academico.php?modulo=principal/listarPortarias&acao={$acao}&ano=2018");

echo montarAbasArray($menu, "/academico/academico.php?modulo=principal/listarPortarias&acao={$acao}&ano=".$ano."");

$subtitulo_modulo = $orgid == ACA_ORGAO_SUPERIOR ? 'Educação Superior' : 'Educação Profissional'; 

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Autorizações - MEC', $subtitulo_modulo );
?>
<!-- biblioteca javascript local -->
<link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
<script type="text/javascript" src="../includes/remedial.js"></script>
<script type="text/javascript" src="../includes/superTitle.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/listagem2.css">
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">
	var params;
	function formatarParametros(){
	    params = Form.serialize($('pesquisar'));
	    
	}
	function desabilitarConteudo( id ){
		var url = 'academico.php?modulo=principal/planodistribuicaocargos&acao=C&carga='+id;
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
	
	function abreportaria( prtid ){
		location.href= 'academico.php?modulo=principal/cadportaria&acao=C&evento=A&prtid=' + prtid;
		
	}
</script>

<!-- corpo da página -->
<form method="POST"  name="formulario" enctype="multipart/form-data">
<input type="hidden" name="evento" id="evento" value="I">
<input type="hidden" name="prtid" id="prtid" value="<?=$prtid?>">
<input type="hidden" name="evento_arquivo" id="evento_arquivo" value=""/>
<input type="hidden" name="arqid" id="arqid" value="<?=$arqid?>">
<input type="hidden" name="anxid" id="anxid" value="<?=$anxid?>">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td width="20%" align='right' class="SubTituloDireita">Número da Portaria:</td>
    <td><?=campo_texto('prtnumero','N','S','',20,100,'','');?></td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita">Número de Controle:</td>
    <td><?=campo_texto('prtid_busca','N','S','',20,100,'','','','','','onkeypress="return somenteNumeros(event);"');?></td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita">Programa:</td>
    <td><?php
	    		$sql = "SELECT
							prgid as codigo,
							prgdsc as descricao
						FROM
							academico.programa
						ORDER BY
							prgdsc";
				$db->monta_combo("prgid", $sql, 'S', "Selecione...", '', '', '', '', 'N', 'prgid');
	    ?></td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita">Data de Inclusão:</td>
    <td><?=campo_data('prtdtinclusao','N','S','','','','');?></td>
</tr>

<tr bgcolor="#DCDCDC">
      <td></td>
      <td colspan="4">
  	  	<input type="button" class="botao" name="btassociar" value="Limpar" onclick="limpar();">	  	  
	  	<input type="button" class="botao" name="btassociar" value="Pesquisar" onclick="submeter('P', '<?=$prtid?>');">  	  	 
    </td>
 </tr>
 <tr bgcolor="#DCDCDC">     
      <td colspan="2"> 	  
  	  	<input type="button" class="botao" name="btassociar" value="Cadastrar Portaria"  onclick="location.href='academico.php?modulo=principal/cadportaria&acao=C&evento=N&tprnivel=1'" <? echo $disabled; ?>>  	  	  	  	 
    </td>
 </tr>
</table>
</form>
<?php

	$academico_lista->listaportarias($filtroprocesso, $acao);

?>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
 <tr bgcolor="#DCDCDC">     
      <td colspan="2"> 	  
  	  	<input type="button" class="botao" name="btassociar" value="Cadastrar Portaria"  onclick="location.href='academico.php?modulo=principal/cadportaria&acao=C&evento=N&tprnivel=1'" <? echo $disabled; ?>>  	  	  	  	 
    </td>
 </tr>
</table>

<script type="text/javascript">

function submeter(evento, prtid){		
		document.getElementById('evento').value = evento;		
		document.formulario.submit();
}

function alterar(evento, prtid){	
	if (confirm("Deseja relmente excluir esta Portaria?")){
		document.getElementById('evento').value = evento;
		document.getElementById('prtid').value = prtid;
		document.formulario.submit();
	}
}

function limpar(){	
	document.getElementById('evento').value = '';	
	document.formulario.submit();
}
function downloadArquivo(evento, anxid, arqid){
	document.getElementById('evento').value = '';
	document.getElementById('evento_arquivo').value = evento;
	document.getElementById('anxid').value = anxid;
	document.getElementById('arqid').value = arqid;
	document.formulario.submit();		
}

</script>
