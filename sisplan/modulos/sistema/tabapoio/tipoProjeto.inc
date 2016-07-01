<?php
$tppid = $_REQUEST['tppid'];

$tipoProjeto = new TipoProjetoPlanejamento( $tppid );

$where = array();
if($_POST['act'] == 'pesquisar'){
    if($_POST['tppnome'])
        $where[] = " tpp.tppnome ILIKE '%{$_POST['tppnome']}%' ";
    if($_POST['tppdescricao'])
        $where[] = " tpp.tppdescricao ILIKE '%{$_POST['tppdescricao']}%' ";
    if($_POST['tppsigla'])
        $where[] = " tpp.tppsigla ILIKE '%{$_POST['tppsigla']}%' ";

    extract($_POST);
}

if ($_POST['act'] == 'excluir'){
    $dado = array('tppstatus' => 'I');
    $tipoProjeto->popularDadosObjeto($dado);
    $tipoProjeto->salvar();
    $tipoProjeto->commit();

    die("<script>
            alert('Operação realizada com sucesso!');
            location.href = '?modulo=sistema/tabapoio/tipoProjeto&acao=A';
        </script>");
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( $titulo_modulo, '' );
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<form method="POST" name="formulario" action="">
<input type="hidden" name="tppid" value="<?php echo $tppid ?>"/>
<input type="hidden" name="evento" value=""/>
<input type=hidden name="act" value="0"/>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
        <td class="SubTituloDireita" width="30%"> Nome:</td>
        <td>
        <?
            echo campo_texto('tppnome','N','','',51,200,'','');
        ?>
        </td>
    </tr>
	<tr>
        <td class="SubTituloDireita" width="30%"> Descrição:</td>
        <td>
        <?
           echo campo_textarea('tppdescricao', 'N', 'S', '', 69, 5, 500 );
        ?>
        </td>
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita"> Sigla:</td>
        <td>
            <?= campo_texto('tppsigla','N','','',10,1,'',''); ?>
        </td>
    </tr>
    <tr style="background-color: #cccccc">
    <td align='right' style="vertical-align:top; width:30%"> </td>
    <td>
        <input type="button" name="Pesquisar" value="Pesquisar" onclick="pesquisar();"/>
        <input type="button" name="Limpar" value="Limpar" onclick="javascript:location.href='?modulo=sistema/tabapoio/tipoProjeto&acao=A'"/>
    </td>
    </tr>
	<tr>
		<td colspan="2">
			<a style="cursor: pointer;" onclick="javascript: window.location='?modulo=sistema/tabapoio/cadTipoProjeto&acao=A';" title="Clique para incluir um novo Tipo de Projeto no sistema">
				<img src="../imagens/incluir.png" style="width: 15px; vertical-align: middle;"/> <b>Incluir novo Tipo de Projeto</b>
			</a>
		</td>
	</tr>
</table>
</form>
<?php


    $arConfig = array("style"           => "width:95%;",
                      "totalLinha"      => false,
                      "totalRegistro"   => true);

    $arCabecalho = array("Ação", "Nome","Descrição","Sigla", "Data de Cadastro");

    $arConfigCol    = array();
	$arConfigCol[0] = array('type' => Lista::TYPESTRING);

    $botoes = '<center> <div style=\'width:60px;\'>
    			<img src=\'/imagens/alterar.gif\' style="cursor:pointer" title="Alterar" onclick="editar( {tppid} )">&nbsp;
            	<img src=\'/imagens/excluir.gif\' style="cursor:pointer" title="Excluir" onclick="excluir( {tppid} )">
               <div> </center>';

    $rs = $tipoProjeto->getLista( $where, true );

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
	window.location = '?modulo=sistema/tabapoio/cadTipoProjeto&acao=A&tppid='+cod;
}

function excluir(cod){
	if ( confirm('Tem certeza que deseja apagar este registro?') ){
        document.formulario.act.value = 'excluir';
        document.formulario.tppid.value = cod;

        document.formulario.submit();
	}
}

</script>

