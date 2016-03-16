<?php

if(!$_SESSION['minc']['mceid'] || !$_SESSION['minc']['entid']){
	?>
	<script>
		alert("Sua sessão expirou. Por favor, entre novamente!");
		location.href='minc.php?modulo=inicio&acao=A';
	</script>
	<?
	exit;
}

if ($_POST['salvar']) {
    global $db;
    $sql = "UPDATE minc.mceparceiro SET paraceite='true' WHERE mceid = {$_SESSION['minc']['mceid']}";
    $db->executar($sql);
    $db->commit();
    exit();
}


define("ESTADO_EM_ANALISE", 548);
include APPRAIZ . "includes/workflow.php";

$sqlDocumento = "SELECT docid FROM minc.mcemaiscultura WHERE mceid = {$_SESSION['minc']['mceid']}";
$rsDocumento = !empty($_SESSION['minc']['mceid']) ? $db->pegaLinha($sqlDocumento) : array();

if (empty($rsDocumento['docid'])) {
    $docid = wf_cadastrarDocumento(TPDID_MAIS_CULTURA, "Documento Mais Cultura #" . $_SESSION['minc']['mceid']);
    $db->executar("UPDATE minc.mcemaiscultura SET docid = '{$docid}' WHERE mceid = {$_SESSION['minc']['mceid']}");
    $db->commit();
} else {
    $docid = $rsDocumento['docid'];
}
?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<?php
require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

$arMnuid = array();
if($docid){
    $sqlDocumento = "select * from workflow.documento where docid = {$docid}";
    $resultDocumento = $db->pegaLinha($sqlDocumento);

  	if ($resultDocumento && !in_array($resultDocumento['esdid'], array(/*ESTADO_DOCUMENTO_AVALIACAO, ESTADO_DOCUMENTO_FINALIZADO, ESTADO_DOCUMENTO_AVALIADO, */ESTADO_DOCUMENTO_ENVIADO_FNDE/*, ESTADO_DOCUMENTO_CORRECAO_CADASTRADOR, ESTADO_DOCUMENTO_AVALIACAO*/))) {
        $arMnuid = array(MNUID_AVALIACAO, MNUID_MONITORAMENTO,MNUID_MONITORAMENTO2);
    }
}else{
	$arMnuid = array( MNUID_AVALIACAO, MNUID_MONITORAMENTO,MNUID_MONITORAMENTO2);
}

$db->cria_aba($abacod_tela, $url,'' , $arMnuid);

$titulo = "Termo de Parceria";
echo monta_titulo($titulo, $subtitulo);

$sql = "SELECT  e.entnumcpfcnpj, e.entnome, en.endlog, endbai, endnum, mn.mundescricao, en.estuf, e2.entnome as representante, 
		e2.entnumcpfcnpj as cpfrepresentante, e2.entnumrg as rgrepresentante
        FROM minc.mceparceiro p
        INNER JOIN minc.mcemaiscultura m using(mceid)
        INNER JOIN entidade.entidade e on e.entid = p.entid
        INNER JOIN entidade.endereco en on en.entid = e.entid
        INNER JOIN territorios.municipio mn using(muncod)
        LEFT JOIN entidade.entidade e2 on e2.entid = p.entidresplegal
        WHERE mceid = {$_SESSION['minc']['mceid']}
          AND parstatus = 'A'";
$rsParceiro = $db->pegaLinha($sql);

$sql = "SELECT e.entnome, en.endlog, en.endbai, en.endnum, mn.mundescricao, en.estuf, e.entcodent
        FROM minc.mcemaiscultura p
        INNER JOIN entidade.entidade e on e.entid = p.entid
        INNER JOIN entidade.endereco en on en.entid = e.entid
        INNER JOIN territorios.municipio mn using(muncod)
		WHERE p.mceid = {$_SESSION['minc']['mceid']}";
                
$rsEscola = $db->pegaLinha($sql);

$sql = "SELECT uexcnpj, uexdescricao 
        FROM minc.mceculturaexecutora c
        INNER JOIN minc.mceunidadeexecutora u using(uexid)
        INNER JOIN minc.mcemaiscultura m using(mceid)
        WHERE m.mceid = {$_SESSION['minc']['mceid']}";
$rsUnidade = $db->pegaLinha($sql);

$sql = "SELECT parid, paraceite
        FROM minc.mceparceiro 
        WHERE mceid = {$_SESSION['minc']['mceid']}";
$rsAceite = $db->pegaLinha($sql);

if ($_SESSION['minc']['mceid']) {

    // Validando inputs de acordo com o estado do documento
    $sqlDocumento = "SELECT docid FROM minc.mcemaiscultura WHERE mceid = {$_SESSION['minc']['mceid']}";
    $rsDocumento = $db->pegaLinha($sqlDocumento);
    $docid = $rsDocumento['docid'];

    if ($docid) {
        $estado = wf_pegarEstadoAtual($docid);

        if ($estado['esdid'] != ESTADO_DOCUMENTO_CADASTRAMENTO) {
            echo "<script>
                    setInterval(function(){
                            var inputs = document.getElementsByTagName('input');
                            var selects = document.getElementsByTagName('select');
                            var textareas = document.getElementsByTagName('textarea');

                            for(var i=0; i<textareas.length; i++){
                                textareas[i].disabled = true;
                             }

                            for(var i=0; i<selects.length; i++){
                                selects[i].disabled = true;
                             }

                            for(var i=0; i<inputs.length; i++){
                                inputs[i].disabled = true;
                             }

                            document.getElementById('btngravar').disabled = 1
                    },100);
                  </script>  
            ";
        }
    }
}
?>

<form id="frmTermo">
    <input type="hidden" name="salvar" value="salvar">
    <table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="listagem2">
        <tr bgcolor="#e7e7e7">
            <td width="5%">&nbsp;</td>
            <td align="left" style="font-size: 13px" valign="top">
                <br/>
                <p>A iniciativa cultural parceiro(a) <?php echo $rsParceiro['entnome']; ?> domiciliado(a)/situado(a) à <?php echo $rsParceiro['endlog']; ?>, número <?php echo $rsParceiro['endnum']; ?>, cidade 
                    <?php echo $rsParceiro['mundescricao'] . '-' . $rsParceiro['estuf']; ?>, inscrito(a) no <? if (strlen($rsParceiro['entnumcpfcnpj']) > 11) echo 'CNPJ';
                    else echo 'CPF'; ?> sob o nº <? if (strlen($rsParceiro['entnumcpfcnpj']) > 11) echo formatar_cnpj($rsParceiro['entnumcpfcnpj']);
                    else echo formatar_cpf($rsParceiro['entnumcpfcnpj']); ?>

                    <?php if (strlen($rsParceiro['entnumcpfcnpj']) > 11): ?>
                        cujo representante legal é <?php echo $rsParceiro['representante']; ?>, portador do CPF <?php echo formatar_cpf($rsParceiro['cpfrepresentante']); ?> 
                        <?php if ($rsParceiro['rgrepresentante']): ?>
                            e RG <?php echo $rsParceiro['rgrepresentante']; ?>
                        <?php endif; ?>
                    <?php endif; ?>

                    , declara estabelecer parceria com a <?php echo $rsEscola['entnome']; ?>, código do INEP <?php echo $rsEscola['entcodent']; ?> cujo endereço é <?php echo $rsEscola['endlog']; ?>, número <?php echo $rsEscola['endnum']; ?>, cidade 
                    <?php echo $rsEscola['mundescricao'] . '-' . $rsEscola['estuf']; ?>, 
                    
                    <!--  
                    que tem como Unidade Executora Própria - UEx o(a) <?php echo $rsUnidade['uexdescricao']; ?>, inscrito(a) no 
                    CNPJ sob o nº <?php echo formatar_cnpj($rsUnidade['uexcnpj']); ?>, 
                    -->
                    
                    para realização do plano de atividade cultural apresentado ao Programa 
                    Mais Cultura nas Escolas.</p>
                <?php
                if (!empty($rsAceite['parid'])) {
                    $estado = wf_pegarEstadoAtual($docid);
                    if ($estado['esdid'] == ESTADO_EM_ANALISE) {
                        $lock = "disabled=disabled";
                    }

                    if ($rsAceite['paraceite'] == 't') {
                        $aceite = "checked='checked'";
                    } else {
                        $aceite = "";
                    }
                    ?>
                    <p>
                        <input type="checkbox" name="partemocompromisso" <?php echo ''; ?> id="partemocompromisso_id" onclick="javascript:salvarTermo();" <?php echo $aceite; ?>/>
                        &nbsp;LEMOS E CONCORDAMOS
                    </p>
                <?php } ?>
                <br/>
            </td>
            <td width="5%">&nbsp;</td>
            <td width="5%">
                <?php 
                if ($docid) {
                    wf_desenhaBarraNavegacao($docid, array('entid'=> $_SESSION['minc']['entid']));
                }
                ?>
            </td>
            <td width="1%">&nbsp;</td>
        </tr>	
    </table>
</form>
<script type="text/javascript" language="javascript">
    function salvarTermo() {
    	console.info(jQuery('#partemocompromisso_id').attr('checked'));
        if (jQuery('#partemocompromisso_id').attr('checked') == true) {
	        jQuery.post(
	        	"minc.php?modulo=principal/termo_compromisso&acao=A", {salvar: 'salvar'}, 
	        	function(data) {
				  history.go(0);
				}
			);
    	}
    }
</script>

<?php if(verificaEncerramentoPlanoEscola()): ?>

	<script>
		jQuery(function(){
			jQuery('input, select, textarea').attr('disabled', true).removeClass('normal').addClass('disabled');
			jQuery('input[type=button]').remove();
		});
	</script>

<?php endif; ?>