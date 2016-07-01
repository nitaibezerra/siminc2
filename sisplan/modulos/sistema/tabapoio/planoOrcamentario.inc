<?php

$id				= $_REQUEST['ploid'];
$planoOrcamentario	= new PlanoOrcamentario($id);

$where = array();
if($_POST['act'] == 'pesquisar'){
    if($_REQUEST['plocod']){
        $where[] = "plo.plocod ILIKE '%{$_REQUEST['plocod']}%'";
    }

    if($_REQUEST['plonome']){
        $where[] = "plo.plonome ILIKE '%{$_REQUEST['plonome']}%'";
    }

    if($_REQUEST['plodescricao']){
        $where[] = "plo.plodescricao ILIKE '%{$_REQUEST['plodescricao']}%'";
    }

    extract($_POST);
}

if ($_POST['act'] == 'excluir'){
    $arDados = array('plostatus' => 'I');
    $planoOrcamentario->popularDadosObjeto($arDados);
    $planoOrcamentario->salvar();
    $planoOrcamentario->commit();

    die("<script>
            alert('Operação realizada com sucesso!');
            location.href = '?modulo=sistema/tabapoio/planoOrcamentario&acao=A';
        </script>");
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( $titulo_modulo, '' );
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<form method="POST" name="formulario" action="">
<input type="hidden" name="ploid" value="<?php echo $id ?>"/>
<input type=hidden name="act" value="0"/>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
     <tr>
         <td align='right' class="SubTituloDireita" style="width:25%;">
            Código:
        </td>
         <td>
	        <?
	            echo campo_texto('plocod','N','','',10,4,'','');
	        ?>
         </td>
     </tr>
     <tr>
         <td align='right' class="SubTituloDireita" style="width:25%;">
            Nome:
        </td>
         <td>
	        <?
	            echo campo_texto('plonome','N','','',51,200,'','');
	        ?>
         </td>
     </tr>
     <tr>
         <td align='right' class="SubTituloDireita" style="width:25%;">
            Descrição:
        </td>
         <td>
	        <?
           		echo campo_textarea('plodescricao', 'N', 'S', '', 69, 5, 500 );
	        ?>
         </td>
     </tr>
    <tr style="background-color: #cccccc">
        <td align='right' style="vertical-align:top; width:25%"> </td>
        <td>
            <input type="button" name="Pesquisar" value="Pesquisar" onclick="pesquisar();"/>
            <input type="button" name="Limpar" value="Limpar" onclick="javascript:location.href='?modulo=sistema/tabapoio/planoOrcamentario&acao=A'"/>
        </td>
    </tr>
	<tr>
		<td colspan="2">
			<a style="cursor: pointer;" onclick="javascript: window.location='?modulo=sistema/tabapoio/cadPlanoOrcamentario&acao=A';" title="Clique para incluir um novo Plano Orçamentário">
				<img src="../imagens/incluir.png" style="width: 15px; vertical-align: middle;"/> <b>Incluir novo Plano Orçamentário</b>
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
    					 "Nome",
    					 "Descrição",
                         "Ação");

    $acao = '<center>
    			<img src=\'/imagens/alterar.gif\' style="cursor:pointer" title="Alterar" onclick="editar( {ploid} )">&nbsp;
            	<img src=\'/imagens/excluir.gif\' style="cursor:pointer" title="Excluir" onclick="excluir( {ploid} )">
             </center>';

    $rs = $planoOrcamentario->getLista( $where, true );

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
	window.location = '?modulo=sistema/tabapoio/cadPlanoOrcamentario&acao=A&ploid='+cod;
}

function excluir(cod){
	if ( confirm('Tem certeza que deseja apagar este registro?') ){
        document.formulario.act.value = 'excluir';
        document.formulario.ploid.value = cod;

        document.formulario.submit();
	}
}

</script>

