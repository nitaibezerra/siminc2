<?php
$mpnid = $_REQUEST['mpnid'];

$metaPnc = new MetaPnc( $mpnid );

$where = array();
if($_POST['act'] == 'pesquisar'){
    if($_POST['mpncod'])
        $where[] = " mpn.mpncod ILIKE '%{$_POST['mpncod']}%' ";
    if($_POST['mpnnome'])
        $where[] = " mpn.mpnnome ILIKE '%{$_POST['mpnnome']}%' ";
    if($_POST['mpndescricao'])
        $where[] = " mpn.mpndescricao ILIKE '%{$_POST['mpndescricao']}%' ";

    extract($_POST);
}

if ($_POST['act'] == 'excluir'){
    $dado = array('mpnstatus' => 'I');
    $metaPnc->popularDadosObjeto($dado);
    $metaPnc->salvar();
    $metaPnc->commit();

    die("<script>
            alert('Operação realizada com sucesso!');
            location.href = '?modulo=sistema/apoio/metaPnc&acao=A';
        </script>");
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( $titulo_modulo, '' );
?>

<form method="POST" name="formulario" action="">
<input type="hidden" name="mpnid" value="<?php echo $mpnid ?>"/>
<input type="hidden" name="evento" value=""/>
<input type=hidden name="act" value="0"/>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
        <td class="SubTituloDireita" width="30%"> Código:</td>
        <td>
        <?
            echo campo_texto('mpncod','N','','',20,2,'','');
        ?>
        </td>
    </tr>
	<tr>
        <td class="SubTituloDireita" width="30%"> Nome:</td>
        <td>
              <?= campo_textarea('mpnnome', 'N', 'S', '', 69, 5, 400 ); ?>
        </td>
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita"> Descrição:</td>
        <td>
              <?= campo_textarea('mpndescricao', 'N', 'S', '', 69, 5, 500 ); ?>
        </td>
    </tr>
    <tr style="background-color: #cccccc">
    <td align='right' style="vertical-align:top; width:30%"> </td>
    <td>
        <input type="button" name="Pesquisar" value="Pesquisar" onclick="pesquisar();"/>
        <input type="button" name="Limpar" value="Limpar" onclick="javascript:location.href='sisplan.php?modulo=sistema/tabapoio/metaPnc&acao=A'"/>
    </td>
    </tr>
	<tr>
		<td colspan="2">
			<a style="cursor: pointer;" onclick="javascript: window.location='sisplan.php?modulo=sistema/tabapoio/cadMetaPnc&acao=A';" title="Clique para incluir uma nova Meta PNC no sistema">
				<img src="../imagens/incluir.png" style="width: 15px; vertical-align: middle;"/> <b>Incluir nova Meta PNC</b>
			</a>
		</td>
	</tr>
</table>
</form>
<?php


    $arConfig = array("style"           => "width:95%;",
                      "totalLinha"      => false,
                      "totalRegistro"   => true);

    $arCabecalho = array("Ação", "Código","Nome","Descrição");

    $arConfigCol    = array();
	$arConfigCol[0] = array('type' => Lista::TYPESTRING);

    $botoes = '<center> <div style=\'width:60px;\'>
    			<img src=\'/imagens/alterar.gif\' style="cursor:pointer" title="Alterar" onclick="editar( {mpnid} )">&nbsp;
            	<img src=\'/imagens/excluir.gif\' style="cursor:pointer" title="Excluir" onclick="excluir( {mpnid} )">
               <div> </center>';

    $where[] = " mpn.prsano = '{$_SESSION['exercicio']}' ";

    $rs = $metaPnc->getLista( $where, true );

    $oLista = new Lista($arConfig);
    $oLista->setCabecalho( $arCabecalho );
    $oLista->setCorpo( $rs, $arConfigCol );
    $oLista->setAcao( $botoes );
    $oLista->show();
?>


<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script language="javascript" type="text/javascript" >

function pesquisar(cod){
    document.formulario.act.value = 'pesquisar';
    document.formulario.submit();
}

function editar(cod){
	window.location = 'sisplan.php?modulo=sistema/tabapoio/cadMetaPnc&acao=A&mpnid='+cod;
}

function excluir(cod){
	if ( confirm('Tem certeza que deseja apagar este registro?') ){
        document.formulario.act.value = 'excluir';
        document.formulario.mpnid.value = cod;

        document.formulario.submit();
	}
}

</script>

