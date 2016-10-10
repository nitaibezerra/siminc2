<?php
$perfis = pegaPerfilGeral();
/**
 * Arquivo com o conteúdo da aba que apresenta o resultado da análise do pedido de limites.
 * $Id: formFinal.inc 96489 2015-04-17 15:03:08Z lindalbertofilho $
 */

if ($pdlid) {
    $sqlFinal = "SELECT cmddsc FROM workflow.comentariodocumento where docid = $docid";
    $justificativaBanco = $db->pegaUm($sqlFinal);
}
?>
<script type="text/javascript" lang="javascript">
$(document).ready(function(){
    // -- Ação do botão gravar Justificativa
    $('#atualizarAnalise').click(function() {
        if ($('#unicod').val() == '') {
            alert('É obrigatório o preenchimento da Unidade Orçamentária.');
            return false;
        }
        if (($('input[name=statusJustificativa]:checked').attr('value') == 'R') && ($("#justificativaDesc").val() == '')) {
            alert('É obrigatório o preenchimento da justificativa quando o limite é rejeitado.');
            return false;
        } else if (($('input[name=statusJustificativa]:checked').attr('value') == 'A') && ($("#pdlnl").val() == '')) {
            alert('É obrigatório o preenchimento da NL quando o limite é aprovado');
            return false;
        } else {
            window.location = "progorc.php?modulo=principal/limite/formulario&acao=A&execucao=inserirAnalise&pagina=formulario"
                    + '&pdlid=' + $('#pdlid').val()
                    + '&pdlnl=' + $('#pdlnl').val()
                    + '&justificativaDesc=' + $('textarea#justificativaDesc').val()
                    + '&statusJustificativa=' + $('input[name=statusJustificativa]:checked').val()
        }
    });
});
</script>
<form id="formAnal" name="formAnal" method="POST" class="form-horizontal" action="#">
    <input type="hidden" name="docid" id="docid" value="<?php echo $docid; ?>" />
    <input type="hidden" name="acao" id="acao" value="salvarAnalise" />
    <input type="hidden" name="pdlid" id="pdlid" value="<?php echo $_REQUEST['pdlid'] ?>">

 <!-- inicio campos: Unidade Orçamentária (UO) e Anexo -->
	<div class="form-group">
	<label for="inputUnidade" class="col-lg-2 control-label">Unidade Orçamentária (UO):</label>
	<div class="col-lg-10">
<?php
	if ($unicod) {
	$sql = <<<DML
SELECT uni.unicod AS codigo,
       uni.unicod || ' - ' || unidsc AS descricao
FROM public.unidade uni
WHERE uni.unicod = '{$unicod}'
DML;
	} else {
		if (in_array(PFL_UO_EQUIPE_TECNICA, $perfis)) {
			$sqlUO = <<<DML
EXISTS (SELECT 1
        FROM progorc.usuarioresponsabilidade rpu
        WHERE rpu.usucpf = '%s'
        AND rpu.pflcod = %d
        AND rpu.rpustatus = 'A'
        AND rpu.unicod  = uni.unicod)
DML;
			$where[] = $whereUO = sprintf($sqlUO, $_SESSION['usucpf'], PFL_UO_EQUIPE_TECNICA);
			$whereUO = " AND {$whereUO}";
    	}
	$sql = "
SELECT uni.unicod AS codigo,
       uni.unicod || ' - ' || unidsc AS descricao
FROM public.unidade uni
WHERE (uni.orgcod = '". CODIGO_ORGAO_SISTEMA. "' OR uni.unicod IN('74902', '73107'))
AND uni.unistatus = 'A'
    {$whereUO}
ORDER BY uni.unicod
";
	}

	$unidadeorcamentaria = $db->pegaLinha($sql);
	$unicodDes = $unidadeorcamentaria ['descricao'];
	$unicodObras = $unidadeorcamentaria ['codigo'];

	$sqlObras = <<<DML
SELECT DISTINCT COUNT(0) AS numero
  FROM obras.obrainfraestrutura oie
    INNER JOIN entidade.entidade e ON oie.entidunidade = e.entid
  WHERE DATE_PART('days', NOW() - obrdtvistoria) > 60
    AND oie.obrpercexec < 100
    AND e.entunicod IS NOT NULL
    AND e.entunicod = '{$unicodObras}'
DML;

	$unidadeobras = $db->pegaLinha($sqlObras);
	$numeroobras = $unidadeobras ['numero'];

	if ($unicod) {
            echo '<p class="form-control-static">'.$unicodDes.'</p>';
	} else {
            $unicod = $_REQUEST['unicod'];
            $db->monta_combo('unicod', $sql, 'S', 'Selecione uma Unidade', null, null, null, null, 'S', 'unicod', null, '', null, 'required="required" class="form-control chosen-select" style="width=100%;"" ', null, (isset($unicod) ? $unicod : null));
	}
?>
	</div>
	</div>
<?php if ($esdid == STDOC_ANALISE_SPO || $esdid == STDOC_ACERTOS_SPO || $esdid == STDOC_ANALISE_SECRETARIA
          || $esdid == STDOC_ACERTOS_SECRETARIA || $esdid == STDOC_ATENDIDO || $esdid == STDOC_RECUSADO): ?>
    <div class="form-group">
        <label for="inputName" class="col-lg-2 control-label">Anexo:</label>
        <div class="col-md-10">
            <div class="btn-group" data-toggle="buttons">
                <input type="hidden" name="anexo" id="anexo" value="<?php echo $_REQUEST['anexo'] ?>"> <p class="form-control-static">Anexo <?php echo $_REQUEST['anexo'] ?></p>
            </div>
        </div>
    </div>
    <?php else: ?>
    <div class="form-group">
        <label for="inputName" class="col-lg-2 control-label">Anexo:</label>
        <div class="col-md-10">
            <div class="btn-group" data-toggle="buttons">
                <label class="btn btn-default <?php echo anexoSelecionado('I'); ?>">
                    <input type="radio" name="anexo" id="anexo" value="I" readonly="readonly"
                           onchange="myFunction(this.value);referencia(this.value);<?php echo $limpaAnexo; ?>" />Anexo I
                </label>
                <label
                    class="btn btn-default <?php echo anexoSelecionado('II'); ?>">
                    <input type="radio" name="anexo" id="anexo_I" value="II"
                           onchange="myFunction(this.value);referencia(this.value);<?php echo $limpaAnexo; ?>" />Anexo II
                </label>
                <label class="btn btn-default <?php echo anexoSelecionado('III'); ?>">
                    <input type="radio" name="anexo" id="anexo_II" value="III"
                           onchange="myFunction(this.value);referencia(this.value);<?php echo $limpaAnexo; ?>" />Anexo III
                </label>
                <label class="btn btn-default <?php echo anexoSelecionado('IV'); ?>">
                    <input type="radio" name="anexo" id="anexo_III" value="IV"
                           onchange="myFunction(this.value);referencia(this.value);<?php echo $limpaAnexo; ?>" />Anexo IV
                </label>
                <label class="btn btn-default <?php echo anexoSelecionado('V'); ?>">
                    <input type="radio" name="anexo" id="anexo_V" value="V"
                           onchange="myFunction(this.value);referencia(this.value);<?php echo $limpaAnexo; ?>" />Anexo V
                </label>
                <label class="btn btn-default <?php echo anexoSelecionado('VI'); ?>">
                    <input type="radio" name="anexo" id="anexo_VI" value="VI"
                           onchange="myFunction(this.value);referencia(this.value);<?php echo $limpaAnexo; ?>" />Anexo VI
                </label>
                <label class="btn btn-default  <?php echo anexoSelecionado('VII'); ?>">
                    <input type="radio" name="anexo" id="anexo_VII" value="VII"
                           onchange="myFunction(this.value);referencia(this.value);<?php echo $limpaAnexo; ?>" />Anexo VII
                </label>
            </div>
        </div>
    </div>
<?php endif; ?>
<!-- fim campos: Unidade Orçamentária (UO) e Anexo -->

    <div class="form-group">
        <label for="inputJustificativa" class="col-lg-2 control-label">Status Final da Análise:</label>
        <div class="col-lg-10 btn-group" data-toggle="buttons">
        <?php if ($esdid == STDOC_ATENDIDO): ?>
            <label class="btn btn-default active">
                <input type="radio" name="statusJustificativa" id="statusJustificativa_A"
                       value="A" id="statusJustificativa_A" />Atendido
            </label>
        <?php elseif ($esdid == STDOC_RECUSADO): ?>
            <label class="btn btn-default active">
                <input type="radio" name="statusJustificativa" id="statusJustificativa_R"
                       value="R" id="statusJustificativa_R" />Recusado
            </label>
        <?php endif; ?>
        </div>
    </div>
<?php if ($esdid != STDOC_RECUSADO): ?>
    <div class="form-group">
        <label for="capvalorfinal" class="col-lg-2 control-label">Nota de Lançamento (NL):</label>
        <div class="col-lg-3">
             <?php
             if (in_array(PFL_CGO_EQUIPE_ORCAMENTARIA, $perfis) || in_array(PFL_SUPER_USUARIO, $perfis) || in_array(STDOC_ANALISE_SPO, $perfis)){
            echo campo_texto('pdlnl', "S", $pdlnl, "Valor Final", 50, '', "######", "", '', '', 0, 'id="pdlnl" class="form-control"', '', '');
             }else{
            ?>
            <input type="text" class="form-control" name="pdlnl" id="pdlnl"
                   value="<?php echo $pdlnl; ?>"
                   readonly="readonly"/> <?php } ?>
        </div>
    </div>
<?php endif ?>
    <div class="form-group">
        <label for="inputJustificativa" class="col-lg-2 control-label">Justificativa:</label>
        <div class="col-lg-10">
            <textarea name="justificativaDesc" cols="70" rows="5" id="justificativaDesc"
                      class="form-control" readonly="readonly"><?php echo $justificativaBanco; ?></textarea>
        </div>
    </div>
    <?php if (in_array(PFL_CGO_EQUIPE_ORCAMENTARIA, $perfis) || in_array(PFL_SUPER_USUARIO, $perfis) || in_array(STDOC_ANALISE_SPO, $perfis)): ?>
    <div class="form-group" align="center">
        <div class="col-lg-10">
            <button class="btn btn-info" id="atualizarAnalise" type="button">Gravar</button>
        </div>
    </div>
<?php else: ?>
    <center>Formulário desabilitado para edição.</center>
<?php endif; ?>
</form>