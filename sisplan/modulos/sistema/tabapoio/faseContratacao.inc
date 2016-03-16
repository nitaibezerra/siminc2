<?php
$facid = $_REQUEST['facid'];

$fase = new FaseContratacao($facid);
extract($fase->getDados());

if ($_POST['act'] == 'salvar'){

    if($_REQUEST['facid'] == ''){
        $_REQUEST['usucpf']          = $_SESSION['usucpf'];
        $_REQUEST['facstatus']       = 'A';
        $_REQUEST['facdatacadastro'] = 'now()';
    }

    $fase->popularDadosObjeto()
          ->salvar();
    $fase->commit();
    $db->sucesso( $_REQUEST['modulo'], '');

}elseif ($_POST['act'] == 'excluir'){
    $dado = array('facstatus' => 'I');
    $fase->popularDadosObjeto($dado)
          ->salvar();
    $fase->commit();

    $db->sucesso( $_REQUEST['modulo'], '');
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( $titulo_modulo, obrigatorio() . 'Indica Campo Obrigatório.' );
?>
<form method="POST" name="formulario" action="">
    <input type="hidden" name="facid" value="<?= $facid; ?>"/>
    <input type=hidden   name="act"   value="0"/>

    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td align='right' class="SubTituloDireita">Nome:</td>
            <td>
                <?= campo_texto('facnome','S','','',100,200,'',''); ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita">Descrição:</td>
            <td>
            <?= campo_textarea( 'facdescricao', 'S', 'S', '', '80', '5', '500', '' , 0, ''); ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita">Ordem:</td>
            <td>
                <?= campo_texto('facordem','S','','',4,4,'####',''); ?>
            </td>
        </tr>
        <tr style="background-color: #cccccc">
        <td align='right' style="vertical-align:top; width:25%"> </td>
        <td>
            <input type="button" name="salvar" value="Salvar" onclick="validar_cadastro();"/>
            <input type="button" name="cancelar" value="Cancelar" onclick="javascript:location.href='?modulo=sistema/tabapoio/faseContratacao&acao=C'"/>
        </td>
        </tr>
    </table>
</form>
<?php


    $arConfig = array("style"           => "width:95%;",
                      "totalLinha"      => false,
                      "totalRegistro"   => true);
    $arCabecalho = array("Ações", "Nome", "Descrição", "Ordem", "Cadastrado por", "Data Cadastro");

    $acao = '<center>
                <img src=\'/imagens/alterar.gif\' style="cursor:pointer" title="Alterar" onclick="editar( {facid} )">
                &nbsp;
                <img src=\'/imagens/excluir.gif\' style="cursor:pointer" title="Excluir" onclick="excluir( {facid} )">
            </center>';

    $rs = $fase->getByWhere();

    $oLista = new Lista($arConfig);
    $oLista->setCabecalho( $arCabecalho );
    $oLista->setCorpo( $rs );
    $oLista->setAcao( $acao );
    $oLista->show();
?>


<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script language="javascript" type="text/javascript" >

function validar_cadastro(cod){
    if (!validaBranco(document.formulario.facnome, 'Nome')) return;
    if (!validaBranco(document.formulario.facdescricao, 'Descrição')) return;
    if (!validaBranco(document.formulario.facordem, 'Ordem')) return;

    document.formulario.act.value = 'salvar';

    document.formulario.submit();
}

function editar(cod){
    document.formulario.act.value = 'carregar';
    document.formulario.facid.value = cod;

    document.formulario.submit();
}

function excluir(cod){
    if ( confirm('Tem certeza que deseja apagar esta Fase de Contratação?') ){
        document.formulario.act.value = 'excluir';
        document.formulario.facid.value = cod;

        document.formulario.submit();
    }
}
</script>