<?php
if ($_POST['act'] == 'excluir' && $_REQUEST['acoid']){
    $acompanhamento = new Acompanhamento($_REQUEST['acoid']);

    $dado = array('acostatus' => 'I');
    $acompanhamento->popularDadosObjeto($dado)
                ->salvar();
    $acompanhamento->commit();

    $db->sucesso( $_REQUEST['modulo'], "&atiid={$_REQUEST['atiid']}&conid={$_REQUEST['conid']}");
    die();
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

if( $_REQUEST['conid'] && $_REQUEST['atiid']){
    $db->cria_aba($abacod_tela, $url, "&atiid={$_REQUEST['atiid']}&conid={$_REQUEST['conid']}");
}

monta_titulo( $titulo_modulo, '' );

include APPRAIZ . 'sisplan/modulos/principal/cabecalhopa.inc';
?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td>
            <a href="#" onclick="novoAcompanhamento();">
                <img src="/imagens/gif_inclui.gif" style="cursor:pointer;" border="0" title="Nova Contratação" align="top">&nbsp;&nbsp;Incluir Acompanhamento
            </a>
        </td>
    </tr>
    <tr>
        <td>
            <?php
                $acompanhamento = new Acompanhamento();
                $rs             = $acompanhamento->getByWhere(array(), $_REQUEST['conid']);

                $arConfig = array("style"           => "width:95%;",
                                  "totalLinha"      => false,
                                  "totalRegistro"   => true);
                $arCabecalho = array("Ações", "Data Inclusão", "Inserido Por", "Situação da Contratação", "Observação");

                $acao = '<center>
                            <img src=\'/imagens/alterar.gif\' style="cursor:pointer" title="Alterar" onclick="editar( {acoid} )">
                            &nbsp;
                            <img src=\'/imagens/excluir.gif\' style="cursor:pointer" title="Excluir" onclick="excluir( {acoid} )">
                        </center>';

                $oLista = new Lista($arConfig);
                $oLista->setCabecalho( $arCabecalho );
                $oLista->setCorpo( $rs );
                $oLista->setAcao( $acao );
                $oLista->show();
            ?>
        </td>
    </tr>
</table>

<form method="POST" name="formulario" action="">
    <input type="hidden" name="acoid" value="""/>
    <input type=hidden   name="act"   value="0"/>
</form>

<script type="text/javascript">
    function novoAcompanhamento(){
        window.location = '?modulo=principal/dadosAcompanhamento&acao=A&atiid=<?= $_REQUEST['atiid']; ?>&conid=<?= $_REQUEST['conid']; ?>';
    }

    function editar(cod){
        window.location = '?modulo=principal/dadosAcompanhamento&acao=A&atiid=<?= $_REQUEST['atiid']; ?>&conid=<?= $_REQUEST['conid']; ?>&acoid='+cod;
    }

    function excluir(cod){
        if ( confirm('Tem certeza que deseja apagar este Acompanhamento?') ){
            document.formulario.act.value = 'excluir';
            document.formulario.acoid.value = cod;

            document.formulario.submit();
        }
    }

</script>