<?php
/**
 * Arquivo da aba análise secretária do pedido de limites.
 * $Id: formAnalise.inc 96489 2015-04-17 15:03:08Z lindalbertofilho $
 */
?>
<script type="text/javascript" lang="javascript">
$(document).ready(function(){
    $('#atualizarSecretaria').click(function() {
        if ($('#pdldatavalidadeobras').val() == '') {
            alert('É obrigatório o preenchimento da Data de validade de obras.');
            return false;
        }
        if ($('textarea#justificativaSec').val() == '') {
            alert('É obrigatório o preenchimento da justificativa');
            return false;
        }

        window.location = "progorc.php?modulo=principal/limite/formulario&acao=A&execucao=inserirSecretaria&pagina=formulario"
                + '&pdlaprovadoobras=' + $('#pdlaprovadoobras').val()
                + '&docid=' + $('#docid').val()
                + '&pdlid=' + $('#pdlid').val()
                + '&pdldatavalidadeobras=' + $('#pdldatavalidadeobras').val()
                + '&justificativaSec=' + $('textarea#justificativaSec').val()
    });

    $("#pdldatavalidadeobras").datepicker({minDate: 0, numberOfMonths: 1, showWeek: true});
<?php if (!$pdldatavalidadeobras): ?>
    $('#pdldatavalidadeobras').mask('99/99/9999');
    $("#pdldatavalidadeobras").datepicker("option", "showAnim", 'drop');
<?php endif; ?>
});
</script>
<form id="formAnal" name="formAnal" method="POST" class="form-horizontal" action="#">
	<input type="hidden" name="docid" id="docid" value="<?php echo $docid; ?>" />
    <input type="hidden" name="acao" id="acao" value="salvarAnalise" />
    <input type="hidden" name="pdlid" id="pdlid" value="<?php echo $_REQUEST['pdlid'] ?>" />
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
            echo '<p class="form-control-static">'.$unicodDes."</p>";
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
        <div class="col-md-10 col-md-offset-2">
            <?php
            $complemento = '';
            if ($esdid != STDOC_ANALISE_SECRETARIA || !in_array(PFL_SECRETARIA, $perfis)) {
                $complemento = 'disabled="disabled"';
            }
            if ($pdlaprovadoobras == 't') {
                $complemento .= ' checked';
            }
            ?>
            <div class="checkbox">
                <label>
                <input type="checkbox" name="pdlaprovadoobras" value="1" id="pdlaprovadoobras" <?php echo $complemento; ?> > Aprovado Módulo de Obras
                </label>
            </div>
        </div>
    </div>
    <div class="form-group">
        <label for="" class="col-lg-2 control-label">Data de Validade do Modulo Obras:</label>
        <div class="col-lg-2">
            <?php
            $complemento = '';
            if ($esdid != STDOC_ANALISE_SECRETARIA || !in_array(PFL_SECRETARIA, $perfis)) {
                $complemento = ' disabled="disabled"';
            }
            ?>
            <input name="pdldatavalidadeobras" type="text" class="form-control" <?php echo $complemento; ?>
                   id="pdldatavalidadeobras" placeholder="00/00/0000" maxlength="12" value="<?php echo $pdldatavalidadeobras; ?>"
                   data-format="dd/MM/yyyy" />
        </div>
    </div>
    <div class="form-group">
        <label for="inputJustificativa" class="col-lg-2 control-label">Observações:</label>
        <div class="col-lg-10">
            <?php
            $complemento = '';
            if ($esdid != STDOC_ANALISE_SECRETARIA || !in_array(PFL_SECRETARIA, $perfis) || in_array(PFL_SUPER_USUARIO, $perfis)) {
                $complemento = 'disabled="disabled"';
            }
            ?>
            <textarea name="justificativaSec" cols="70" rows="5" <?php echo $complemento; ?>
                      id="justificativaSec" class="form-control"><?php echo $observacao ?></textarea>
        </div>
    </div>
<?php if (in_array(PFL_SECRETARIA, $perfis) || 1 == $_SESSION['superuser']): ?>
    <div class="form-group" align="center">
        <div class="col-lg-10">
            <button class="btn btn-danger" type="button" id="voltar" onclick="voltarLista()">Voltar</button>
        <?php if ($esdid == STDOC_ANALISE_SECRETARIA): ?>
            <button class="btn btn-info" id="atualizarSecretaria" type="button">Gravar</button>
        <?php endif; ?>
        </div>
    </div>
<?php else: ?>
    <center>Formulário desabilitado para edição.</center>
<?php endif; ?>
</form>
