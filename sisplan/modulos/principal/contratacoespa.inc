<?php
if ($_POST['act'] == 'excluir' && $_REQUEST['conid']){
    $contratacao = new Contratacao($_REQUEST['conid']);

    $dado = array('constatus' => 'I');
    $contratacao->popularDadosObjeto($dado)
                ->salvar();
    $contratacao->commit();

    $db->sucesso( $_REQUEST['modulo'],"&atiid={$_REQUEST['atiid']}");
    die();
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( $titulo_modulo, '' );

include APPRAIZ . 'sisplan/modulos/principal/cabecalhopa.inc';
?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td>
            <a href="#" onclick="novaContratacao();">
                <img src="/imagens/gif_inclui.gif" style="cursor:pointer;" border="0" title="Nova Contratação" align="top">&nbsp;&nbsp;Nova Contratação
            </a>
        </td>
    </tr>
    <tr>
        <td>
            <?php
                $contratacao = new Contratacao();

                $campos = "CASE
		                       WHEN numtotalf.numtotalfases = numcc.numconcluidacancelada          THEN '<center><span class=\"divCor\" style=\"background-color:blue;\" title=\"Concluída\">&nbsp;&nbsp;&nbsp;&nbsp;</span></center>'
		                       WHEN numamaior10.numatrasomaior10 > 0                               THEN '<center><span class=\"divCor\" style=\"background-color:red;\" title=\"Crítico\">&nbsp;&nbsp;&nbsp;&nbsp;</span></center>'
		                       WHEN numamenor10.numatrasomenor10maiormenos1 > 0                    THEN '<center><span class=\"divCor\" style=\"background-color:#DADF00;\" title=\"Merece Atenção\">&nbsp;&nbsp;&nbsp;&nbsp;</span></center>'
		                       WHEN numamenormenos1.numatrasomenormenos1 > 0                       THEN '<center><span class=\"divCor\" style=\"background-color:green;\" title=\"Estável\">&nbsp;&nbsp;&nbsp;&nbsp;</span></center>'
		                       ELSE '<center>N/A</center>'
		                   END AS situacao,
		                   con.conobjeto,
		                   TO_CHAR(con.condatainicio, 'DD/MM/YYYY') AS condatainicio,
		                   TO_CHAR(con.condatatermino, 'DD/MM/YYYY') AS condatatermino,
		                   itr.itrdsc, --modalidade
		                   con.convalorestimado,
		                   usu.usunome,
		                   TO_CHAR(con.condatacadastro, 'DD/MM/YYYY') AS condatacadastro,
		                   con.conid";

                $rs          = $contratacao->getByWhere2(array("ati.atiid = {$_REQUEST['atiid']}"),$campos);

                $arConfig = array("style"           => "width:95%;",
                                  "totalLinha"      => false,
                                  "totalRegistro"   => true);

                $arCabecalho = array("Ações",
                                     "Situação",
                                     "Objeto",
                                     "Data Início",
                                     "Data prevista de Término",
                                     "Modalidade",
                                     "Valor Previsto",
                                     "Responsável",
                                     "Data Cadatro");

                $acao = '<center>
                            <img src=\'/imagens/alterar.gif\' style="cursor:pointer" title="Alterar" onclick="editar( {conid} )">
                            <img src=\'/imagens/excluir.gif\' style="cursor:pointer" title="Excluir" onclick="excluir( {conid} )">
                        </center>';

                $arParamCol[1] = array("type" => Lista::TYPESTRING);

                $oLista = new Lista($arConfig);
                $oLista->setCabecalho( $arCabecalho );
                $oLista->setCorpo( $rs, $arParamCol );
                $oLista->setAcao( $acao );
                $oLista->show();
            ?>
        </td>
    </tr>
</table>

<form method="POST" name="formulario" action="">
    <input type="hidden" name="conid" value="""/>
    <input type=hidden   name="act"   value="0"/>
</form>

<script type="text/javascript">
    function novaContratacao(){
        window.location = '?modulo=principal/dadosContratacao&acao=A&atiid=<?= $_REQUEST['atiid']; ?>';
    }

    function editar(cod){
        window.location = '?modulo=principal/dadosContratacao&acao=A&atiid=<?= $_REQUEST['atiid']; ?>&conid='+cod;
    }

    function excluir(cod){
        if ( confirm('Tem certeza que deseja excluir esta Contratação?') ){
            document.formulario.act.value = 'excluir';
            document.formulario.conid.value = cod;

            document.formulario.submit();
        }
    }

</script>