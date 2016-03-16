<script type="text/javascript" src="../includes/prototype-1.6.0.3.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="javascript" type="text/javascript" src="../includes/micoxAjax.js"></script>
<script language="javascript" type="text/javascript" >

jQuery.noConflict();

</script>

<?php

$permissao_formulario = 'S';
$id     = $_REQUEST['ndpid'];
$modelo = new NaturezaDespesa($id);
extract($modelo->getDados());

if ($_POST['act'] == 'salvar'){

	//recuperando os codigos
	$ctecod = substr($_REQUEST['ndpcod'], 0, 1);
	$gndcod = substr($_REQUEST['ndpcod'], 1, 1);
	$mapcod = substr($_REQUEST['ndpcod'], 2, 2);
	$edpcod = substr($_REQUEST['ndpcod'], 4, 2);
	$sbecod = substr($_REQUEST['ndpcod'], 6, 2);

	//verificando se os mesmos existem
	$qtdCtecod = $db->pegaUm( "SELECT COUNT(ctecod) FROM categoriaeconomica WHERE ctecod = '{$ctecod}'" );
	$qtdGndcod = $db->pegaUm( "SELECT COUNT(gndcod) FROM gnd WHERE gndcod = '{$gndcod}'" );
	$qtdMapcod = $db->pegaUm( "SELECT COUNT(mapcod) FROM modalidadeaplicacao WHERE mapcod = '{$mapcod}'" );
	$qtdEdpcod = $db->pegaUm( "SELECT COUNT(edpcod) FROM elementodespesa WHERE edpcod = '{$edpcod}'" );

	if( $qtdCtecod == 0 || $qtdGndcod == 0 || $qtdMapcod == 0 || $qtdEdpcod == 0 ){
		echo "<script> alert('O código informado é inválido.'); </script>";
		extract($_REQUEST);
	}else{

	    $arDados = array(
	                        'ndpid'     			=> $_REQUEST['ndpid'],
	                        'ndpcod'    			=> $_REQUEST['ndpcod'],
	                        'ndpdsc'				=> $_REQUEST['ndpdsc'],
	                        'ndpsubelementostatus' 	=> 'A',
	                        'ndpstatus' 			=> 'A',
	                        'ctecod' 				=> $ctecod,
	                        'gndcod' 				=> $gndcod,
	                        'mapcod' 				=> $mapcod,
	                        'edpcod' 				=> $edpcod,
	                        'sbecod' 				=> $sbecod,
	    					'ndpano'				=> '2007'
	                      );

	    $modelo->popularDadosObjeto($arDados)->salvar();
	    $modelo->commit();
	    $db->sucesso( 'sistema/tabapoio/naturezaDespesa', '');
	}
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$titulo = (empty($id) ? 'Cadastro de Natureza de Despesa' : 'Alteração de Natureza de Despesa');
monta_titulo( $titulo, obrigatorio() . 'Indica Campo Obrigatório.' );
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<form method="POST" name="formulario" action="">
<input type="hidden" name="ndpid" value="<?= $ndpid; ?>"/>
<input type=hidden name="act" value="0"/>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
     <tr>
         <td align='right' class="SubTituloDireita" style="width:25%;">
            Código:
        </td>
         <td>
	        <?
	            echo campo_texto('ndpcod','S','','',10,8,'########','');
	        ?>
         </td>
     </tr>
     <tr>
         <td align='right' class="SubTituloDireita" style="width:25%;">
            Descrição:
        </td>
         <td>
	        <?
           		echo campo_textarea('ndpdsc', 'S', 'S', '', 69, 5, 255 );
	        ?>
         </td>
     </tr>
    <tr style="background-color: #cccccc">
        <td align='right' style="vertical-align:top; width:25%">
            &nbsp;
        </td>
        <td>
            <input type="button" name="salvar" value="Salvar" onclick="validar_cadastro();"/>
            <input type="button" name="Voltar" value="Voltar" onclick="javascript:location.href='?modulo=sistema/tabapoio/naturezaDespesa&acao=A'"/>
        </td>
    </tr>
</table>
</form>

<script language="javascript" type="text/javascript" >


function validar_cadastro(){

    if (!validaBranco(document.formulario.ndpcod, 'Código')) return;
    if (!validaBranco(document.formulario.ndpdsc, 'Descrição')) return;

    document.formulario.act.value = 'salvar';
    document.formulario.submit();
}



</script>

