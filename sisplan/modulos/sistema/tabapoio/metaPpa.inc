<?php
$mppid = $_REQUEST['mppid'];

$metaPpa = new MetaPpa( $mppid );

$where = array();
if($_POST['act'] == 'pesquisar'){
    if($_POST['mppcod'])
        $where[] = " mpp.mppcod ILIKE '%{$_POST['mppcod']}%' ";
    if($_POST['mppnome'])
        $where[] = " mpp.mppnome ILIKE '%{$_POST['mppnome']}%' ";
    if($_POST['mppdescricao'])
        $where[] = " mpp.mppdescricao ILIKE '%{$_POST['mppdescricao']}%' ";

    extract($_POST);
}

if ($_POST['act'] == 'excluir'){
    $dado = array('mppstatus' => 'I');
    $metaPpa->popularDadosObjeto($dado);
    $metaPpa->salvar();
    $metaPpa->commit();

    die("<script>
            alert('Operação realizada com sucesso!');
            location.href = 'sisplan.php?modulo=sistema/tabapoio/metaPpa&acao=A';
        </script>");
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( $titulo_modulo, '' );
?>

<form method="POST" name="formulario" action="">
<input type="hidden" name="mppid" value="<?php echo $mppid ?>"/>
<input type="hidden" name="evento" value=""/>
<input type=hidden name="act" value="0"/>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
        <td class="SubTituloDireita" width="30%"> Código:</td>
        <td>
        <?
            echo campo_texto('mppcod','N','','',20,2,'','');
        ?>
        </td>
    </tr>
	<tr>
        <td class="SubTituloDireita" width="30%"> Nome:</td>
        <td>
              <?= campo_textarea('mppnome', 'N', 'S', '', 69, 5, 400 ); ?>
        </td>
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita"> Descrição:</td>
        <td>
              <?= campo_textarea('mppdescricao', 'N', 'S', '', 69, 5, 500 ); ?>
        </td>
    </tr>
    <tr style="background-color: #cccccc">
    <td align='right' style="vertical-align:top; width:30%"> </td>
    <td>
        <input type="button" name="Pesquisar" value="Pesquisar" onclick="pesquisar();"/>
        <input type="button" name="Limpar" value="Limpar" onclick="javascript:location.href='?modulo=sistema/apoio/metaPpa&acao=A'"/>
    </td>
    </tr>
	<tr>
		<td colspan="2">
			<a style="cursor: pointer;" onclick="javascript: window.location='sisplan.php?modulo=sistema/tabapoio/cadMetaPpa&acao=A';" title="Clique para incluir uma nova Meta PPA no sistema">
				<img src="../imagens/incluir.png" style="width: 15px; vertical-align: middle;"/> <b>Incluir nova Meta PPA</b>
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
	$arConfigCol[0] = array('type'  => Lista::TYPESTRING);

    $botoes = '<center> <div style=\'width:60px;\'>
    			<img src=\'/imagens/alterar.gif\' style="cursor:pointer" title="Alterar" onclick="editar( {mppid} )">&nbsp;
            	<img src=\'/imagens/excluir.gif\' style="cursor:pointer" title="Excluir" onclick="excluir( {mppid} )">
               <div> </center>';

    $where[] = " mpp.prsano = '{$_SESSION['exercicio']}' ";

    $rs = $metaPpa->getLista( $where, true );

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
	window.location = 'sisplan.php?modulo=sistema/tabapoio/cadMetaPpa&acao=A&mppid='+cod;
}

function excluir(cod){
	if ( confirm('Tem certeza que deseja apagar este registro?') ){
        document.formulario.act.value = 'excluir';
        document.formulario.mppid.value = cod;

        document.formulario.submit();
	}
}

</script>

