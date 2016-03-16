<?php

if($_REQUEST['req']!=''){
    $_REQUEST['req']($_REQUEST);
}

$_REQUEST['curid'] = $_REQUEST['curid'] ? $_REQUEST['curid'] : $_SESSION['catalogo']['curid'];
$_SESSION['catalogo']['curid'] = $_REQUEST['curid'] ? $_REQUEST['curid'] : $_SESSION['catalogo']['curid'];

$dadosCurso = recuperaDadosCurso($_REQUEST);
//$_POST['modid'] = $dadosCurso['modid'];
$pflcods    = pegaPerfis($_SESSION['usucpf']);

//if( (!$db->testa_superuser() && !in_array(PERFIL_CONSULTA,$pflcods) && !in_array(PERFIL_ADMINISTRADOR,$pflcods)) && $_SESSION['catalogo']['curid'] != '' ){
//	$arrCoords = recuperaCoordenacaoResponssavel();
//	if( !in_array($dadosCurso['coordid'],$arrCoords) ){
//		echo "<script>
//				alert('Acesso negado.');
//				window.location = \"catalogocurso.php?modulo=principal/listaCursos&acao=A\";
//			  </script>";
//	}
//}

$docid = prePegarDocid( $_REQUEST['curid'] );

if( $_SESSION['catalogo']['curid'] ){
    $permissoes = testaPermissao();
}else{
    $permissoes['gravar'] = true;
}

//$_POST['redid'] = recuperaRedesCurso($_REQUEST);

if(is_array($dadosCurso)){
    extract($dadosCurso);
    $curchtot = $dadosCurso['curchpre']+$dadosCurso['curchdist'];
}
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

if($_SESSION['catalogo']['curid']){
    monta_abas();
}else{
    monta_abas(57394);
}

switch ($modid){
    case MODALIDADE_PRESENCIAL:
        $limitHr = 360;
        break;
    case MODALIDADE_SEMIPRESENCIAL:
        $limitHr = 180;
        break;
    case MODALIDADE_DISTANCIA:
        $limitHr = 180;
        break;
    default:
        $limitHr = 0;
        break;
}

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="geral/funcoes_curso.js"></script>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="listagem2">
    <tr class="SubTituloEsquerda">
        <td style="FONT-SIZE: 12pt;">
            <center>
                Dados Gerais
            </center>
        </td>
    </tr>
    <tr class="FundoTitulo">
        <td>
            <center><img border="0" align="top" src="../imagens/obrig.gif" >Indica campo obrigatório</center>
        </td>
    </tr>
</table>
<?php
if( $_REQUEST['curid'] ){
    montaCabecalho($_REQUEST['curid']);
}
?>
<form method="post" enctype="multipart/form-data" name="frmCatalogo" id="frmCatalogo">
<input type="hidden" value="" id="req" name="req"/>
<input type="hidden" value="" id="link" name="link"/>
<input type="hidden" value="<?=$_SESSION['catalogo']['curid'] ?>" id="curid" name="curid"/>
<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
    <tr>
        <td class="SubTituloDireita" width="15%">
            Status
        </td>
        <td>
            <?php monta_radio_status($_POST); ?>
        </td>
    </tr>
</table>
<br>
<table align="center" width="95%" border="0" cellpadding="5" cellspacing="1" class="listagem2">
    <tr class="FundoTitulo">
        <td>
            <b>PARTE 1</b>
        </td>
    </tr>
</table>
<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
    <tr>
        <td class="SubTituloDireita" width="15%">
            Código / Área Temática
        </td>
        <td>
            <?php monta_combo_areaTematica( $_POST ); ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">
            Código / Nome do Curso
        </td>
        <td>
            <?=campo_texto('curid', 'N', 'N', 'Nome do Curso', '6', '6','',''); ?>&nbsp;
            <?=campo_texto('curdesc', 'S', ($permissoes['gravar'] ? 'S' : 'N'), 'Nome do Curso', '70', '255','',''); ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">
            Etapa de ensino a que se<br> destina
        </td>
        <td>
            <?php combo_popup_etapaEnsino( $_POST ); ?>
            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">
            Nivel do Curso
        </td>
        <td>
            <?php monta_combo_nivelCurso( $_POST ); ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita" width="15%">
            Rede
        </td>
        <td>
            <?php monta_checkbox_rede($dadosCurso); ?>
            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">
            Objetivo
        </td>
        <td>
            <?=campo_textarea('curobjetivo', 'S', ($permissoes['gravar'] ? 'S' : 'N'), 'Objetivo', '75', '4', '5000'); ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">
            Descrição do Curso
        </td>
        <td>
            <?=campo_textarea('curementa', 'S', ($permissoes['gravar'] ? 'S' : 'N'), 'Descrição do Curso', '75', '4', '5000'); ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">
            Fundamentos Teóricos Metodológicos
        </td>
        <td>
            <?=campo_textarea('curfunteome', 'N', ($permissoes['gravar'] ? 'S' : 'N'), 'Fundamentos Teóricos Metodológicos', '75', '4', '5000'); ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">
            Título Concedido /<br> Certificação
        </td>
        <td>
            <?=campo_textarea('curcertificado', 'S', ($permissoes['gravar'] ? 'S' : 'N'), 'Título Concedido/Certificação', '75', '4', '5000'); ?>
        </td>
    </tr>
</table>
<br>
<table align="center" width="95%" border="0" cellpadding="5" cellspacing="1" class="listagem2">
    <tr class="FundoTitulo">
        <td>
            <b>PARTE 2</b>
        </td>
    </tr>
</table>
<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
    <tr>
        <td class="SubTituloDireita" width="15%">
            Metodologia
        </td>
        <td>
            <?=campo_textarea('curmetodologia', 'N', ($permissoes['gravar'] ? 'S' : 'N'), 'Metodologia', '75', '4', '5000'); ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">
            Modalidade de Ensino
        </td>
        <td>
            <?php monta_checkboxcheckbox_modalidadeCurso( $dadosCurso ); ?>
            <!--				<?=monta_combo_modalidadeCurso($_POST); ?>-->
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">
            Curso com oferta interestadual
        </td>
        <td>
            <?php monta_radio_ofertaNacional($_POST); ?>
            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">
            Carga Horária
        </td>
        <td>
            <?php
            if($_SESSION['catalogo']['curid']){
                $antcurchmim = $db->pegaUm("SELECT curchmim
												   FROM catalogocurso.curso 
												   WHERE curid = ".$curid);
                $antcurchmax = $db->pegaUm("SELECT curchmax
												   FROM catalogocurso.curso 
												   WHERE curid = ".$curid);
            }
            ?>
            <input type="hidden" name="antcurchmim"  id="antcurchmim"  value="<?=$antcurchmim ?>"/>
            <input type="hidden" name="antcurchmax"  id="antcurchmax"  value="<?=$antcurchmax ?>"/>
            <br>
            <b>Carga horária total do curso:</b><br>&nbsp;&nbsp;
            Mínimo:&nbsp;
            <?=campo_texto('curchmim', 'S', ($permissoes['gravar'] ? 'S' : 'N'), '', '6', '15', '[#]', '', '', '', '', 'id="curchmim"','','','this.value=mascaraglobal(\'[#]\',this.value);') ?>&nbsp;Horas.&nbsp;
            Máximo:&nbsp;
            <?=campo_texto('curchmax', 'S', ($permissoes['gravar'] ? 'S' : 'N'), '', '6', '15', '[#]', '', '', '', '', 'id="curchmax"','','','this.value=mascaraglobal(\'[#]\',this.value);') ?>&nbsp;Horas.<br><br>
            <div id="preexigida">
                <b>Carga horária presencial exigida(%):</b><br>&nbsp;&nbsp;
                Mínimo:&nbsp;
                <?=campo_texto('curpercpremim', 'S', ($permissoes['gravar'] ? 'S' : 'N'), '', '3', '5', '###', '', '', '', '', 'id="curpercpremim"','','','this.value=mascaraglobal(\'[#]\',this.value);') ?>&nbsp;%.&nbsp;
                Máximo:&nbsp;
                <?=campo_texto('curpercpremax', 'S', ($permissoes['gravar'] ? 'S' : 'N'), '', '3', '5', '###', '', '', '', '', 'id="curpercpremax"','','','this.value=mascaraglobal(\'[#]\',this.value);') ?>&nbsp;%.<br><br>
            </div>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">
            Número de estudantes por<br> turma
        </td>
        <td>
            <b>Presencial:</b><br>&nbsp;&nbsp;
            Ideal
            <?=campo_texto('curnumestudanteidealpre', 'S', ($permissoes['gravar'] ? 'S' : 'N'), '', '15', '15', '[#]', '', '', '', '', 'id="curnumestudanteidealpre"','','','this.value=mascaraglobal(\'[#]\',this.value);') ?>&nbsp;
            Mínimo
            <?=campo_texto('curnumestudanteminpre', 'S', ($permissoes['gravar'] ? 'S' : 'N'), '', '15', '15', '[#]', '', '', '', '', 'id="curnumestudanteminpre"','','','this.value=mascaraglobal(\'[#]\',this.value);') ?>&nbsp;
            Máximo
            <?=campo_texto('curnumestudantemaxpre', 'S', ($permissoes['gravar'] ? 'S' : 'N'), '', '15', '15', '[#]', '', '', '', '', 'id="curnumestudantemaxpre"','','','this.value=mascaraglobal(\'[#]\',this.value);') ?>
            <br><br>
            <b>A Distância:</b><br>&nbsp;&nbsp;
            Ideal
            <?=campo_texto('curnumestudanteidealdist', 'S', ($permissoes['gravar'] ? 'S' : 'N'), '', '15', '15', '[#]', '', '', '', '', 'id="curnumestudanteidealdist"','','','this.value=mascaraglobal(\'[#]\',this.value);') ?>&nbsp;
            Mínimo
            <?=campo_texto('curnumestudantemindist', 'S', ($permissoes['gravar'] ? 'S' : 'N'), '', '15', '15', '[#]', '', '', '', '', 'id="curnumestudantemindist"','','','this.value=mascaraglobal(\'[#]\',this.value);') ?>&nbsp;
            Máximo
            <?=campo_texto('curnumestudantemaxdist', 'S', ($permissoes['gravar'] ? 'S' : 'N'), '', '15', '15', '[#]', '', '', '', '', 'id="curnumestudantemaxdist"','','','this.value=mascaraglobal(\'[#]\',this.value);') ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">
            Periodicidade de Monitoramento
        </td>
        <td>
            A cada &nbsp;
            <?=campo_texto('curqtdmonitora', 'S', ($permissoes['gravar'] ? 'S' : 'N'), '', '10', '10', '[#]', '', '', '', '', 'id="curqtdmonitora"','','','this.value=mascaraglobal(\'[#]\',this.value);') ?>&nbsp;
            <?php monta_radio_uniMedMonitora($_POST); ?>
            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">
            Infraestrutura Recomendada
        </td>
        <td>
            <?=campo_textarea('curinfra', 'N', ($permissoes['gravar'] ? 'S' : 'N'), 'Infraestrutura Recomendada', '75', '4', '5000'); ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">
            Sala de Recursos Multifuncionais
        </td>
        <td>
            <?php monta_radio_multi($_POST); ?>
            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">
            Localização da Escola
        </td>
        <td>
            <?php monta_combo_localizacaoEscola( $_POST ); ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">
            Localização Diferenciada da Escola
        </td>
        <td>
            <?php monta_combo_localizacaoDiferenciadaEscola( $_POST ); ?>
        </td>
    </tr>
</table>
<br>
<table align="center" width="95%" border="0" cellpadding="5" cellspacing="1" class="listagem2">
    <tr class="FundoTitulo">
        <td>
            <b>PARTE 3</b>
        </td>
    </tr>
</table>
<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
    <tr>
        <td class="SubTituloDireita" width="15%">
            Custo/Aluno estimado
        </td>
        <td>
            <?php $curcustoaluno = str_replace('.',',',$curcustoaluno); ?>
            R$ &nbsp;<?=campo_texto('curcustoaluno', 'S', ($permissoes['gravar'] ? 'S' : 'N'), '', '15', '15', '[#],##', '', '', '', '', 'id="curcustoaluno"','','','this.value=mascaraglobal(\'[#],##\',this.value);') ?>
        </td>
    </tr>
</table>
<br>
<table align="center" width="95%" border="0" cellpadding="5" cellspacing="1" class="listagem2">
    <tr class="FundoTitulo">
        <td>
            <b>PARTE 4</b>
        </td>
    </tr>
</table>
<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
    <tr>
        <td class="SubTituloDireita">
            Documentos Adicionais
        </td>
        <td td="tdAnexos">
            <img border="0" align="absmiddle" title="Incluir Arquivo" style="cursor: pointer" src="../imagens/gif_inclui.gif" id="addArquivo" />
            Incluir Arquivo<br><br>
            <table class="tabela" style="width: 80%" align="left" border="0" cellpadding="5" cellspacing="1" id="arquivos">
                <tr>
                    <td class="SubTituloDireita" width="50%">
                        <center>Descrição</center>
                    </td>
                    <td class="SubTituloDireita">
                        <center>Arquivo</center>
                    </td>
                    <td class="SubTituloDireita" width="5%"><center> - </center></td>
                </tr>
                <?=recuperaArquivos($_REQUEST); ?>
                <tr id="bordainferior">
                    <td></td>
                    <td></td>
                </tr>
            </table>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita" width="15%">
            Período de Oferta do Curso
        </td>
        <td>
            Início
            <?=campo_data2('curinicio','S',($permissoes['gravar'] ? 'S' : 'N'),'','DD/MM/YYYY','','', $curinicio, '', '', 'curinicio' ) ?>
            &nbsp;Fim
            <?=campo_data2('curfim','N',($permissoes['gravar'] ? 'S' : 'N'),'','DD/MM/YYYY','','', $curfim, '', '', 'curfim' ) ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">
            Coordenação responsável no<br> MEC ou na CAPES
        </td>
        <td>
            <?php monta_combo_resp( $_POST ); ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">
            Responsável pela ultima<br> alteração
        </td>
        <td>
            <?=campo_texto('usunome', 'N', 'N', '', '55', '55', '', '') ?>&nbsp;
            <?=campo_texto('hicdata', 'N', 'N', 'DD/MM/YYYY', '17', '17', '', '') ?> &nbsp;
            <a id="btoHistorico">Historico [+]</a>
            <div id="historico" style="display:none">
                <?php
                if($_REQUEST['curid']){
                    $sql = "SELECT
									usunome, 
									to_char(hicdata,'DD/MM/YYYY - HH24:MI:SS') as data
								FROM 
									catalogocurso.historicocurso h
								INNER JOIN seguranca.usuario u ON u.usucpf = h.usucpf
								WHERE 
									curid = ".$_REQUEST['curid']."
								ORDER BY
									data DESC";
                    $hts = $db->carregar($sql);
                }
                $hts = $hts ? $hts : Array();
                $cabecalho = Array("Usuário","Data da Alteração");
                $db->monta_lista_array($hts, $cabecalho, 50, 20, '', '100%', '',Array());
                ?>
            </div>
        </td>
    </tr>
</table>
<br>
<table align="center" width="95%" border="0" cellpadding="5" cellspacing="1" class="listagem2">
    <tr class="FundoTitulo">
        <td>
            <b>&nbsp;</b>
        </td>
    </tr>
</table>
<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
    <tr>
        <td colspan="2">
            <center>
                <input type="button" id="voltar"  value="Voltar"/>
                <input type="button" id="salvar"  value="Salvar" <?=(!$permissoes['gravar']  ? 'disabled' : '') ?>/>
                <input type="button" id="salvarC" value="Salvar e Continuar" <?=(!$permissoes['gravar']  ? 'disabled' : '') ?>/>
                <input type="button" id="proximo" value="Próximo"/>
            </center>
        </td>
    </tr>
</table>
</form>