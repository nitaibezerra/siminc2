<?php

$id				    = $_REQUEST['ndpid'];
$naturezaDespesa	= new NaturezaDespesa($id);

$where = array();
if($_POST['act'] == 'pesquisar'){
    if($_REQUEST['ndpcod']){
        $where[] = "ndp.ndpcod = '{$_REQUEST['ndpcod']}'";
    }

    if($_REQUEST['ndpdsc']){
        $where[] = "ndp.ndpdsc ILIKE '%{$_REQUEST['ndpdsc']}%'";
    }

    extract($_POST);
}

if ($_POST['act'] == 'excluir'){
    $arDados = array('ndpsubelementostatus' => 'I');
    $naturezaDespesa->popularDadosObjeto($arDados);
    $naturezaDespesa->salvar();
    $naturezaDespesa->commit();

    die("<script>
            alert('Operação realizada com sucesso!');
            location.href = '?modulo=sistema/tabapoio/naturezaDespesa&acao=A';
        </script>");
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( $titulo_modulo, '' );
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<form method="POST" name="formulario" action="">
<input type="hidden" name="ndpid" value="<?php echo $id ?>"/>
<input type=hidden name="act" value="0"/>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
     <tr>
         <td align='right' class="SubTituloDireita" style="width:25%;">
            Código:
        </td>
         <td>
	        <?
	            echo campo_texto('ndpcod','N','','',10,8,'########','');
	        ?>
         </td>
     </tr>
     <tr>
         <td align='right' class="SubTituloDireita" style="width:25%;">
            Descrição:
        </td>
         <td>
	        <?
           		echo campo_textarea('ndpdsc', 'N', 'S', '', 69, 3, 255 );
	        ?>
         </td>
     </tr>
    <tr style="background-color: #cccccc">
        <td align='right' style="vertical-align:top; width:25%"> </td>
        <td>
            <input type="button" name="Pesquisar" value="Pesquisar" onclick="pesquisar();"/>
            <input type="button" name="Voltar" value="Voltar" onclick="javascript:location.href='?modulo=sistema/tabapoio/ptresnatureza&acao=C'"/>
        </td>
    </tr>
	<tr>
		<td colspan="2">
			<a style="cursor: pointer;" onclick="javascript: window.location='?modulo=sistema/tabapoio/cadNaturezaDespesa&acao=A';" title="Clique para incluir uma nova Natureza de Despesa">
				<img src="../imagens/incluir.png" style="width: 15px; vertical-align: middle;"/> <b>Incluir nova Natureza de Despesa</b>
			</a>
		</td>
	</tr>
</table>
</form>
<?php
    $arConfig = array("style"           => "width:95%;",
                      "totalLinha"      => false,
                      "totalRegistro"   => true);

    $arCabecalho = array("Ação",
    					 "Código",
    					 "Descrição");

    $acao = '<center>
    			<img src=\'/imagens/alterar.gif\' style="cursor:pointer" title="Alterar" onclick="editar( {ndpid} )">&nbsp;
            	<img src=\'/imagens/excluir.gif\' style="cursor:pointer" title="Excluir" onclick="excluir( {ndpid} )">
             </center>';

    $rs = $naturezaDespesa->getLista( $where, true );

    $oLista = new Lista($arConfig);
    $oLista->setCabecalho( $arCabecalho );
    $oLista->setCorpo( $rs );
    $oLista->setAcao( $acao );
    $oLista->show();
?>


<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script language="javascript" type="text/javascript" >

function pesquisar(){
    document.formulario.act.value = 'pesquisar';
    document.formulario.submit();
}

function editar(cod){
	window.location = '?modulo=sistema/tabapoio/cadNaturezaDespesa&acao=A&ndpid='+cod;
}

function excluir(cod){
	if ( confirm('Tem certeza que deseja apagar este registro?') ){
        document.formulario.act.value = 'excluir';
        document.formulario.ndpid.value = cod;

        document.formulario.submit();
	}
}

</script>

