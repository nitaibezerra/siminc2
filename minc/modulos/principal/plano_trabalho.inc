<?php
if (!$_SESSION['minc']['mceid'] || !$_SESSION['minc']['entid']) {
    ?>
    <script>
        alert("Sua sessão expirou. Por favor, entre novamente!");
        location.href = 'minc.php?modulo=inicio&acao=A';
    </script>
    <?php
    exit;
}

//submit
if ($_POST) {
    $ptrid = $_POST['ptrid'];
    //insere/altera plano de trabalho
    if (!$ptrid) {
		$sql = "UPDATE minc.mceparceiro SET paraceite = 'false' where parid in (
				SELECT parid FROM minc.mceparceiro WHERE mceid = ". $_SESSION['minc']['mceid'] .")";
    	$db->executar($sql);
    	
    	$sql = "INSERT INTO minc.mceplanotrabalho(
                        mceid, ptrano, ptrobjetivogeral, ptrjustificativa, 
                        ptrresultadosesperados, ptrquantidadealuno, 
                        ptrquantidadeprofessor, ptrquantidadepessoa, ptrquantidadefamiliar,ptrproduto,ptrdescricaproduto,
                        descdesenvolvido, descoqueseradesenv, desccomunidadeescolar, desccomunidadelocal, descrelprojetoescola, ptrcomunidadelocal)
                VALUES (" . $_SESSION['minc']['mceid'] . ", 
                            " . $_SESSION['exercicio'] . ",
                            " . ($_POST['ptrobjetivogeral'] ? "'" . substr($_POST['ptrobjetivogeral'],0,3000) . "'" : 'null') . ",
                            " . ($_POST['ptrjustificativa'] ? "'" . substr($_POST['ptrjustificativa'],0,2000) . "'" : 'null') . ",
                        " . ($_POST['ptrresultadosesperados'] ? "'" . substr($_POST['ptrresultadosesperados'],0,1000) . "'" : 'null') . ", 
                        " . ($_POST['ptrquantidadealuno'] ? $_POST['ptrquantidadealuno'] : 'null') . ", 
                        " . ($_POST['ptrquantidadeprofessor'] ? $_POST['ptrquantidadeprofessor'] : 'null') . ", 
                        " . ($_POST['ptrquantidadepessoa'] ? $_POST['ptrquantidadepessoa'] : 'null') . ", 
                        " . ($_POST['ptrquantidadefamiliar'] ? $_POST['ptrquantidadefamiliar'] : 'null') . ",
                        " . ($_POST['ptrproduto'] == 'S' ? "'t'" : ($_POST['ptrproduto'] == 'N' ? "'f'" : 'null')) . ",
                        '" . substr(($_POST['ptrdescricaproduto']),0,400) . "',
                        '" . substr($_POST['descdesenvolvido'],0,2000) . "',
                        '" . substr($_POST['descoqueseradesenv'],0,2000) . "',
                        '" . substr($_POST['desccomunidadeescolar'],0,1500) . "',
                        '" . substr($_POST['desccomunidadelocal'],0,400) . "',
                        '" . substr($_POST['descrelprojetoescola'],0,2000) . "',
                        " . ($_POST['ptrcomunidadelocal'] == 'S' ? "'t'" : ($_POST['ptrcomunidadelocal'] == 'N' ? "'f'" : 'null')) . ")
                         returning ptrid";
        $ptrid = $db->pegaUm($sql);
    } else {
		$sql = "UPDATE minc.mceparceiro SET paraceite = 'false' where parid in (
				SELECT parid FROM minc.mceparceiro WHERE mceid = ". $_SESSION['minc']['mceid'] .")";
    	$db->executar($sql);

    	$sql = "UPDATE minc.mceplanotrabalho
                        SET 
                            ptrobjetivogeral=" . ($_POST['ptrobjetivogeral'] ? "'" . substr($_POST['ptrobjetivogeral'],0,3000) . "'" : 'null') . ", 
                            ptrjustificativa=" . ($_POST['ptrjustificativa'] ? "'" . substr($_POST['ptrjustificativa'],0,2000) . "'" : 'null') . ", 
                            ptrresultadosesperados=" . ($_POST['ptrresultadosesperados'] ? "'" . substr($_POST['ptrresultadosesperados'],0,1000) . "'" : 'null') . ", 
                            ptrquantidadealuno=" . ($_POST['ptrquantidadealuno'] ? $_POST['ptrquantidadealuno'] : 'null') . ", 
                            ptrquantidadeprofessor=" . ($_POST['ptrquantidadeprofessor'] ? $_POST['ptrquantidadeprofessor'] : 'null') . ", 
                            ptrquantidadepessoa=" . ($_POST['ptrquantidadepessoa'] ? $_POST['ptrquantidadepessoa'] : 'null') . ", 
                            ptrquantidadefamiliar=" . ($_POST['ptrquantidadefamiliar'] ? $_POST['ptrquantidadefamiliar'] : 'null') . ",
                            ptrproduto=" . ($_POST['ptrproduto'] == 'S' ? "'t'" : ($_POST['ptrproduto'] == 'N' ? "'f'" : 'null')) . ",
                            ptrdescricaproduto='" . substr($_POST['ptrdescricaproduto'],0,400) . "',
                            descdesenvolvido='" . substr($_POST['descdesenvolvido'],0,2000) . "',
                            descoqueseradesenv='" . substr($_POST['descoqueseradesenv'],0,2000) . "',
                            desccomunidadeescolar='" . substr($_POST['desccomunidadeescolar'],0,1500) . "',
                            desccomunidadelocal='" . substr($_POST['desccomunidadelocal'],0,400) . "',
                            descrelprojetoescola='" . substr($_POST['descrelprojetoescola'],0,2000) . "',
                            ptrcomunidadelocal=" . ($_POST['ptrcomunidadelocal'] == 'S' ? "'t'" : ($_POST['ptrcomunidadelocal'] == 'N' ? "'f'" : 'null')) . "
                        WHERE ptrid = $ptrid";
        $db->executar($sql);
    }

    if (is_array($_REQUEST['chk_eixo'])) {
        $sqlEixo = '';
        if ($_POST['ptrid']) {
            $sqlEixo .= "delete from minc.mceplanoeixo where ptrid = {$ptrid}; ";
        }
        foreach ($_REQUEST['chk_eixo'] as $eixo) {
            $sqlEixo .= "insert into minc.mceplanoeixo
							(extid,ptrid)
						 values
						 	({$eixo}, {$ptrid}); ";
        }
        $db->executar($sqlEixo);
    }

    //Previsão Orçamentária
    $sql = "DELETE FROM minc.mceprevisaoorcamentaria WHERE ptrid = $ptrid";
    $db->executar($sql);

    $sql = "INSERT INTO minc.mceprevisaoorcamentaria (ptrid, promaterialconsumo,  procontservcultural, procontservdiverso,  prolocacao, promaterialpermanente) 
            	   VALUES ($ptrid,
            	    	   " . ($_POST['promaterialconsumo'] ? "'" . str_replace(',', '.', str_replace('.', '', $_POST['promaterialconsumo'])) . "'" : 'null') . ",
            	    	   " . ($_POST['procontservcultural'] ? "'" . str_replace(',', '.', str_replace('.', '', $_POST['procontservcultural'])) . "'" : 'null') . ",
            	    	   " . ($_POST['procontservdiverso'] ? "'" . str_replace(',', '.', str_replace('.', '', $_POST['procontservdiverso'])) . "'" : 'null') . ",
            	    	   " . ($_POST['prolocacao'] ? "'" . str_replace(',', '.', str_replace('.', '', $_POST['prolocacao'])) . "'" : 'null') . ",
            	    	   " . ($_POST['promaterialpermanente'] ? "'" . str_replace(',', '.', str_replace('.', '', $_POST['promaterialpermanente'])) . "'" : 'null') . ")";

    $db->executar($sql);

    //Cronograma
    $sql = "DELETE FROM minc.mcecronogramaatividade WHERE ptrid = $ptrid";
    $db->executar($sql);

    if ($_POST['cradescricao'][1]) {
        foreach ($_POST['cradescricao'] as $k => $v) {
            if ($v) {
                if ($_POST['crames_' . $k]) {
                    foreach ($_POST['crames_' . $k] as $v2) {
                        if ($v2) {
                            $sql = "INSERT INTO minc.mcecronogramaatividade(ptrid, cradescricao, crames, craano) VALUES ($ptrid, '" . substr( $v ,0,200) . "', " . $v2 . ", " . $_SESSION['exercicio'] . " )";
                            $db->executar($sql);
                        }
                    }
                }
            }
        }
    }
    $db->commit();
    print "
        <script>
            alert('Operação realizada com sucesso!');
            location.href='minc.php?modulo=principal/plano_trabalho&acao=A';
	</script>
    ";
    exit();
}

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";

$sqlDocumento = "SELECT docid FROM minc.mcemaiscultura WHERE mceid = {$_SESSION['minc']['mceid']}";
$rsDocumento = !empty($_SESSION['minc']['mceid']) ? $db->pegaLinha($sqlDocumento) : array();

$docid = $rsDocumento['docid'];

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

$titulo = "Mais Cultura nas Escolas";
$dsctitulo = 'Plano de Atividade Cultural';

monta_titulo($titulo, $dsctitulo);

echo cabecalho();

//carrega dados para edição
$sql = "SELECT ptrid, ptrano, ptrobjetivogeral, ptrjustificativa, 
		       ptrresultadosesperados, ptrquantidadealuno, 
		       ptrquantidadeprofessor, ptrquantidadepessoa, ptrquantidadefamiliar,
		       ptrproduto, ptrdescricaproduto,descdesenvolvido, descoqueseradesenv, desccomunidadeescolar,  
		       desccomunidadelocal, descrelprojetoescola, descelemtodialogo, m.docid, ptrcomunidadelocal
	 	FROM minc.mceplanotrabalho p 
	 	INNER JOIN minc.mcemaiscultura m ON m.mceid = p.mceid  
	 	WHERE p.mceid = " . $_SESSION['minc']['mceid'];
$dados = $db->pegaLinha($sql);
if ($dados)
    extract($dados);

if ($ptrid) {

    $sql = "SELECT promaterialconsumo, procontservcultural, procontservdiverso, prolocacao, promaterialpermanente
		 	FROM minc.mceprevisaoorcamentaria
		 	WHERE ptrid = $ptrid";
    $dados = $db->pegaLinha($sql);
    if ($dados) {
        extract($dados);
        if ($promaterialconsumo)
            $promaterialconsumo = number_format($promaterialconsumo, 2, ",", ".");
        if ($procontservcultural)
            $procontservcultural = number_format($procontservcultural, 2, ",", ".");
        if ($procontservdiverso)
            $procontservdiverso = number_format($procontservdiverso, 2, ",", ".");
        if ($prolocacao)
            $prolocacao = number_format($prolocacao, 2, ",", ".");
        if ($promaterialpermanente)
            $promaterialpermanente = number_format($promaterialpermanente, 2, ",", ".");
    }
}

include_once APPRAIZ . 'includes/workflow.php';

if ($_SESSION['minc']['mceid']) {

    // Validando inputs de acordo com o estado do documento
    $sqlDocumento = "SELECT docid
                                    FROM minc.mcemaiscultura 
                                    WHERE mceid = {$_SESSION['minc']['mceid']}";
    $rsDocumento = $db->pegaLinha($sqlDocumento);
    $docid = $rsDocumento['docid'];

    if ($docid) {
        $estado = wf_pegarEstadoAtual($docid);

         if(($estado['esdid'] != ESTADO_DOCUMENTO_CADASTRAMENTO) && ($estado['esdid'] != ESTADO_DOCUMENTO_CORRECAO_CADASTRADOR)){
            echo "<script>
                    window.onload = function() {
                        var inputs = document.getElementsByTagName('input')
                          , selects = document.getElementsByTagName('select')
                          , textareas = document.getElementsByTagName('textarea')
                          , button = document.getElementById('btngravar');

                        if (textareas) {
                            for(var i=0; i<textareas.length; i++){
                                textareas[i].setAttribute('disabled', true);
                            }
                        }

                        if (selects) {
                            for(var i=0; i<selects.length; i++){
                                selects[i].setAttribute('disabled', true);
                            }
                        }

                        if (inputs) {
                            for(var i=0; i<inputs.length; i++){
                                inputs[i].setAttribute('disabled', true);
                            }
                        }

                        if (button) {
                            button.setAttribute('disabled', true);
                        }
                    }
                  </script>  
            ";
        }
    }
}
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
    jQuery.noConflict();
    jQuery(function() {
        jQuery('[name=ptrproduto]').click(function() {
            if (this.value == 'S') {
                jQuery('#tr_descricao_produto').show();
            } else {
                jQuery('#tr_descricao_produto').hide();
                jQuery('#ptrdescricaproduto').val('');
            }
        });

        jQuery('[name=ptrcomunidadelocal]').click(function() {
            if (this.value == 'S') {
                jQuery('#tr_envolvimento').show();
            } else {
                jQuery('#tr_envolvimento').hide();
                jQuery('#desccomunidadelocal').val('');
            }

        });
    });


</script>
<form id="formulario" name="formulario" action="" method="post">
    <input type="hidden" name="ptrid" value="<?= $ptrid ?>">
    <table border="0" width="95%" align="center"  cellpadding="2" cellspacing="1">
        <tr>
            <td class="SubTituloCentro" colspan="4">Descrição do Plano de Atividade Cultural</td>
        </tr>
        <tr>
            <td width="20%" class="SubTituloDireita"><b>Eixo Temático:</b></td>
            <td>
                <?php
                $sql = "SELECT extid, extdescricao, extexplicacao FROM minc.mceeixotematico WHERE extstatus = 'A' ORDER BY 2";
                $eixo = $db->carregar($sql);
                foreach ($eixo as $a) {
                    $checado = '';
                    if ($ptrid) {
                        $checado = $db->pegaUm("SELECT extid FROM minc.mceplanoeixo WHERE ptrid = {$ptrid} AND extid = " . $a['extid']);
                        if ($checado)
                            $checado = 'checked';
                    }
                    echo '<p align="justify">&nbsp;<input id="chk_eixo" type="checkbox" name="chk_eixo[]" value="' . $a['extid'] . '" ' . $checado . '>&nbsp;';
                    echo '<b>' . $a['extdescricao'] . ':</b> ' . $a['extexplicacao'] . '</p>';
                }
                ?>				
            </td>
            <td valign="middle"><img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"></td>
            <td valign="top" align="center" width="7%">
                &nbsp;
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Objetivo Geral:</b><br>
                <p align="justify">(Nesse campo abordar o que o projeto como um todo quer alcançar, mobilizar ou proporcionar à escola e à comunidade em que ela está inserida, para além dos objetivos específicos visados em cada atividade proposta. Por exemplo: oficinas de teatro podem ter por objetivo específico familiarizar participantes com expressão corporal ou textual, a depender de sua abordagem; já o Plano de Atividade Cultural pode ter por objetivo montar uma peça sobre as origens do bairro em que a escola está localizada e, assim sendo, seu objetivo geral não é a realização da peça propriamente dita, mas desenvolver um processo de trabalho e aprendizado que proporcione apropriação do lugar pela escola/ comunidade.)</p>
            </td>
            <td colspan="3"><?= campo_textarea('ptrobjetivogeral', 'S', 'S', '', 80, 5, 3000); ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Justificativa:</b>
                <p align="justify">(Descreva o contexto e a realidade sociocultural e econômica da comunidade em que a escola está inserida e justifique a importância do desenvolvimento do projeto. Fale sobre as peculiaridades da escola, da iniciativa cultural parceira e do público a que se destina o projeto.)</p>
            </td>
            <td colspan="3"><?= campo_textarea('ptrjustificativa', 'S', 'S', '', 80, 5, 2000); ?></td>
        </tr>
        <tr>
            <td colspan="4" class="subtituloCentro">METODOLOGIA</td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>O que será desenvolvido:</b></td>
            <td colspan="3"><?= campo_textarea('descdesenvolvido', 'S', 'S', '', 80, 5, 2000); ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Como será desenvolvido:</b></td>
            <td colspan="3"><?= campo_textarea('descoqueseradesenv', 'S', 'S', '', 80, 5, 2000); ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Como a comunidade escolar (pais, alunos, professores e funcionários da escola) será integrada ao projeto:</b></td>
            <td colspan="3"><?= campo_textarea('desccomunidadeescolar', 'S', 'S', '', 80, 5, 1500); ?></td>
        </tr>
<!--	
        <tr>
                <td class="SubTituloDireita"><b>Haverá envolvimento da comunidade local (moradores, estabelecimentos comerciais, espaços públicos, espaços coletivos diversos que se encontram entorno da escola) com o projeto?</b></td>
                <td colspan="3"><//?= campo_textarea('desccomunidadelocal', 'S', 'S', '', 80, 5, 1500); ?></td>
        </tr>
-->
        <tr>
            <td class="SubTituloDireita"><b>Haverá envolvimento da comunidade local (moradores, estabelecimentos comerciais, espaços públicos, espaços coletivos diversos que se encontram entorno da escola) com o projeto?</b></td>
            <td colspan="3">				
                <input type="radio" name="ptrcomunidadelocal" value="S" <?php echo $ptrcomunidadelocal == 't' ? 'checked' : '' ?>>&nbsp;Sim&nbsp;
                <input type="radio" name="ptrcomunidadelocal" value="N" <?php echo $ptrcomunidadelocal == 'f' ? 'checked' : '' ?>>&nbsp;Não
                &nbsp;&nbsp;&nbsp;
                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
            </td>
        </tr>
        <tr id="tr_envolvimento" style="display:none;">
            <td class="SubTituloDireita"><b>Envolvimento:</b></td>
            <td colspan="3">
                <?= campo_textarea('desccomunidadelocal', 'S', 'S', '', 80, 5, 400); ?>
            </td>

        <tr>
            <td class="SubTituloDireita"><b>Qual a relação entre o projeto proposto e o projeto político pedagógico da escola:</b></td>
            <td colspan="3"><?= campo_textarea('descrelprojetoescola', 'S', 'S', '', 80, 5, 2000); ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Haverá produto ao final desta parceria entre a escola e a iniciativa parceira? (exposição, vídeo, peça teatral, etc):</b></td>
            <td colspan="3">				
                <input type="radio" name="ptrproduto" value="S" <?php echo $ptrproduto == 't' ? 'checked' : '' ?>>&nbsp;Sim&nbsp;
                <input type="radio" name="ptrproduto" value="N" <?php echo $ptrproduto == 'f' ? 'checked' : '' ?>>&nbsp;Não
                &nbsp;&nbsp;&nbsp;
                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
            </td>
        </tr>
        <tr id="tr_descricao_produto" style="display:none;">
            <td class="SubTituloDireita"><b>Descreva o produto:</b></td>
            <td colspan="3">
                <?= campo_textarea('ptrdescricaproduto', 'S', 'S', '', 80, 5, 400); ?>
            </td>
        </tr>
<!-- 		<tr>
                <td class="SubTituloDireita"><b>Elementos do  diálogo entre o plano de trabalho proposto e o projeto pedagógico da escola:</b></td>
                <td colspan="3"><? //= campo_textarea('descelemtodialogo', 'S', 'S', '', 80, 5, 1000);  ?></td>
        </tr> -->
        <tr>
            <td colspan="4" class="subtituloCentro">PLANO DE TRABALHO DETALHADO</td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Estimativa de Pessoas Envolvidas:</b></td>
            <td colspan="3">
                <table width="34%" class="listagem">
                    <tr class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; background-color: #dcdcdc;">
                        <td><b>Envolvidos</b></td>
                        <td><b>Quantidade</b></td>
                    </tr>
                    <tr style="background-color: #F7F7F7;">
                        <td>Estudantes</td>
                        <td><?= campo_texto('ptrquantidadealuno', 'S', 'S', '', 8, 6, '######', '', '', '', '', '', 'somaPessoas(this)'); ?></td>
                    </tr>
                    <tr style="background-color: #F7F7F7;">
                        <td>Professores</td>
                        <td><?= campo_texto('ptrquantidadeprofessor', 'S', 'S', '', 8, 6, '######', '', '', '', '', '', 'somaPessoas(this)'); ?></td>
                    </tr>
                    <tr style="background-color: #F7F7F7;">
                        <td>Familiares</td>
                        <td><?= campo_texto('ptrquantidadefamiliar', 'S', 'S', '', 8, 6, '######', '', '', '', '', '', 'somaPessoas(this)'); ?></td>
                    </tr>
                    <tr style="background-color: #F7F7F7;">
                        <td>Pessoas da Comunidade</td>
                        <td><?= campo_texto('ptrquantidadepessoa', 'S', 'S', '', 8, 6, '######', '', '', '', '', '', 'somaPessoas(this)'); ?></td>
                    </tr>
                    <tr style="background-color: #F7F7F7;">
                        <td>
                            Total
                        </td>
                        <td>
                            <?
                            if ($ptrquantidadealuno)
                                $totalenvolvidos += $ptrquantidadealuno;
                            if ($ptrquantidadeprofessor)
                                $totalenvolvidos += $ptrquantidadeprofessor;
                            if ($ptrquantidadefamiliar)
                                $totalenvolvidos += $ptrquantidadefamiliar;
                            if ($ptrquantidadepessoa)
                                $totalenvolvidos += $ptrquantidadepessoa;
                            echo campo_texto('totalenvolvidos', 'S', 'N', '', 8, 6, '######', ''); 
                            ?>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Resultados Esperados:</b></td>
            <td colspan="3"><?= campo_textarea('ptrresultadosesperados', 'S', 'S', '', 80, 5, 1000); ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Cronograma de Ação:</b>
                <p align="justify">(O cronograma complementa o que foi exposto no item metodologia de trabalho e deve conter a atividade (o que) e o período (quando), pelo período mínimo de 6 (seis) meses letivos, ainda que não consecutivos, a contar do mês da efetivação do repasse de recursos.)</p>
            </td>
            <td colspan="3">
                <table>
                    <tr>
                        <td>
                            <table id="tblcronograma" width="400" class="listagem">
                                <tr class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; background-color: #dcdcdc;">
                                    <td colspan="13" align="center">
                                        <b>CRONOGRAMA <?= $_SESSION['exercicio'] + 1 ?></b>
                                    </td>
                                </tr>
                                <tr class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; background-color: #dcdcdc;">
                                    <td >
                                        <b>O que?</b>
                                    </td>
                                <? for ($i = 1; $i < 13; $i++) { ?>
                                        <td>
                                            &nbsp;
                                            <b><?= substr(mes_extenso($i), 0, 3) ?></b>
                                            &nbsp;
                                        </td>
                                <? } ?>
                                </tr>
                                <?
                                if ($ptrid) {
                                    //$sql = "SELECT craid, ptrid, cradescricao, crames, craano FROM minc.mcecronogramaatividade where ptrid = $ptrid";
                                    $sql = "SELECT cradescricao 
                                            FROM minc.mcecronogramaatividade 
                                            where ptrid = $ptrid
                                            and craano = " . $_SESSION['exercicio'] . "
                                            group by cradescricao
                                            order by 1";
                                    $cronograma = $db->carregar($sql);
                                }

                                $ctCronograma = count($cronograma);
                                if ($ctCronograma <= 1)
                                    $ctCronograma = 1;

                                for ($ct = 1; $ct <= $ctCronograma; $ct++) {
                                    ?>
                                    <tr>
                                        <td>
                                        <?//= campo_texto('cradescricao[' . $ct . ']', 'N', 'S', '', 20, 200, '', '', '', '', '', '', '', $cronograma[$ct - 1]['cradescricao']); ?>
                                        <?= campo_texto('cradescricao[' . $ct . ']', 'N', 'S', $cronograma[$ct - 1]['cradescricao'], 94, 200, '', '', '', '', '', 'class="example"', '', $cronograma[$ct - 1]['cradescricao']); ?>
                                        </td>
                                        <?
                                        for ($i = 1; $i < 13; $i++) {

                                            if ($ptrid) {
                                                $sql = "SELECT crames 
                                                        FROM minc.mcecronogramaatividade 
                                                        where ptrid = $ptrid
                                                        and craano = " . $_SESSION['exercicio'] . "
                                                        and cradescricao = '" . addslashes($cronograma[$ct - 1]['cradescricao']) . "'
                                                        and crames = $i";
                                                $mes = $db->pegaUm($sql);
                                            }

                                            $checado = '';
                                            if ($mes)
                                                $checado = 'checked';
                                            ?>
                                            <td>
                                                <input type="checkbox" name="crames_<?= $ct ?>[]" id="crames_<?= $ct ?>_<?= $i ?>" value="<?= $i ?>" <?= $checado ?>>
                                            </td>
                                        <? } ?>
                                    </tr>
                            <? } ?>				
                            </table>
                            <div>
                                <a href="javascript:addLinha()">[+] Adicionar</a>
                            </div>
                        </td>
                        <td><img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"></td>
                    </tr>
                </table>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Previsão Orçamentária:</b></td>
            <td colspan="3">
                <?php

                $sql = "select e.fk_cod_entidade as codibge,count(m.fk_cod_aluno) as qtd_alunos
                        from educacenso_2013.tab_dado_escola e
                        inner join educacenso_2013.tab_matricula m ON e.fk_cod_entidade = m.fk_cod_entidade
                        join entidade.entidade ee on ee.entcodent::char varying = e.fk_cod_entidade::char varying
                        where ee.entid IN ({$_SESSION['minc']['entid']})
                        group by 1";
                
                //ver($_SESSION['minc']);
                $mcequantidadealuno = $db->pegaLinha($sql);

                if ($mcequantidadealuno['qtd_alunos'] <= 500) {
                    $saldoorcamento = 20000;
                    $promaterialpermanente = 2000;
                } else
                if ($mcequantidadealuno['qtd_alunos'] >= 501 && $mcequantidadealuno['qtd_alunos'] <= 1000) {
                    $saldoorcamento = 21000;
                    $promaterialpermanente = 2500;
                } else
                if ($mcequantidadealuno['qtd_alunos'] > 1000) {
                    $saldoorcamento = 22000;
                    $promaterialpermanente = 3000;
                } else {
                    $saldoorcamento = 0;
                    $promaterialpermanente = 0;
                }
                ?>
                <table width="36%" class="listagem" border="0">
                    <tr class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; background-color: #dcdcdc;">
                        <td><b>O que</b></td>
                        <td width="37%"><b>Valor</b></td>
                    </tr>
                    <tr style="background-color: #F7F7F7;">
                        <td>Total Orçamento</td>
                        <td>
                            <?php
                            $totalorcamento = number_format($saldoorcamento, 2, ",", ".");
                            echo campo_texto('totalorcamento', 'S', 'N', '', 17, 15, '', '');
                            ?>
                        </td>
                    </tr>
                    <tr style="background-color: #F7F7F7;">
                        <td>Aquisição de Materiais de consumo</td>
                        <td><?= campo_texto('promaterialconsumo', 'S', 'S', '', 17, 15, '###.###.###,##', '', '', '', '', '', 'atualizaOrc(this)'); ?></td>
                    </tr>
                    <tr style="background-color: #F7F7F7;">
                        <td>Contratação de serviços culturais necessários às atividades artísticas e pedagógicas</td>
                        <td><?= campo_texto('procontservcultural', 'S', 'S', '', 17, 15, '###.###.###,##', '', '', '', '', '', 'atualizaOrc(this)'); ?></td>
                    </tr>
                    <tr style="background-color: #F7F7F7;">
                        <td>Contratação de serviços diversos</td>
                        <td><?= campo_texto('procontservdiverso', 'S', 'S', '', 17, 15, '###.###.###,##', '', '', '', '', '', 'atualizaOrc(this)'); ?></td>
                    </tr>
                    <tr style="background-color: #F7F7F7;">
                        <td>Locação de instrumentos, transporte, equipamentos</td>
                        <td><?= campo_texto('prolocacao', 'S', 'S', '', 17, 15, '###.###.###,##', '', '', '', '', '', 'atualizaOrc(this)'); ?></td>
                    </tr>
                    <tr style="background-color: #F7F7F7;">
                        <td>Aquisição de materiais permanentes e equipamentos</td>
                        <td>
                            <?php
                            $promaterialpermanente = number_format($promaterialpermanente, 2, ",", ".");
                            echo campo_texto('promaterialpermanente', 'S', 'N', '', 17, 15, '###.###.###,##', '', '', '', '', '', 'atualizaOrc(this)');
                            ?>
                        </td>
                    </tr>
                    <tr style="background-color: #F7F7F7;">
                        <td>Saldo Orçamento</td>
                        <td>
                            <?php
                            if ($promaterialconsumo)
                                $saldoorcamento -= str_replace(",", ".", str_replace(".", "", $promaterialconsumo));
                            if ($procontservcultural)
                                $saldoorcamento -= str_replace(",", ".", str_replace(".", "", $procontservcultural));
                            if ($procontservdiverso)
                                $saldoorcamento -= str_replace(",", ".", str_replace(".", "", $procontservdiverso));
                            if ($prolocacao)
                                $saldoorcamento -= str_replace(",", ".", str_replace(".", "", $prolocacao));
                            if ($promaterialpermanente)
                                $saldoorcamento -= str_replace(",", ".", str_replace(".", "", $promaterialpermanente));
                            $saldoorcamento = number_format($saldoorcamento, 2, ",", ".");
                            echo campo_texto('saldoorcamento', 'S', 'N', '', 17, 15, '', ''); 
                            ?>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
        <tr>
            <td align="center" width="100%" style="background: rgb(238, 238, 238);" colspan="4">
                <table width="100%">
                    <tr>
                        <td class="SubTituloCentro" colspan="2">
                            <?php 
                                 $perfis = arrayPerfil();
                                 if(in_array(PERFIL_MINC_CADASTRADOR, $perfis) || in_array(PERFIL_MINC_SUPER_USUARIO, $perfis)){
                                     echo "<input type=\"button\" name=\"btSalvarC1\" id=\"btSalvarC1\" value=\"Salvar\" onclick=\"validaFormPlan();\">";
                                 }else{
                                     echo "<input type=\"button\" name=\"btSalvarC1\" id=\"btSalvarC1\" value=\"Salvar\" disabled=\"disabled\">";
                                 }
                            ?>
                        </td>
                        <td align="right">&nbsp;</td>
                    </tr>
                </table>
            </td>
        </tr>
    </table>
</form>

<script type="text/javascript" src="/includes/prototype.js"></script> 
<script>

    function addLinha() {
        var totalLinhas = document.getElementById('tblcronograma').rows.length;
        var id = totalLinhas - 1;
        var linhanovo = document.getElementById('tblcronograma').insertRow(totalLinhas);
        linhanovo.insertCell(0).innerHTML = "<input type='text' style='text-align:;' name='cradescricao[" + id + "]' size='21' maxlength='200' value='' onmouseover='MouseOver(this);' onfocus='MouseClick(this);this.select();' onmouseout='MouseOut(this);' onblur='MouseBlur(this);' title='' class='normal'>";
        for (i = 1; i < 13; i++) {
            linhanovo.insertCell(i).innerHTML = "<input type='checkbox' name='crames_" + id + "[]' id='crames_" + id + "_" + i + "' value='" + i + "'>";
        }
    }

    function validaFormPlan() {
        var d = document.formulario;
        if (jQuery('#chk_eixo:checked').length == 0) {
            alert('Selecione ao menos um Eixo Temático.');
            jQuery('[name=chk_eixo]').focus();
            return false;
        }
        if (jQuery('#ptrobjetivogeral').val() == "") {
            alert('O campo Objetivo é obrigatório.');
            jQuery('#ptrobjetivogeral').focus();
            return false;
        }
        if (jQuery('#ptrjustificativa').val() == "") {
            alert('O campo Justificativa é obrigatório.');
            jQuery('#ptrjustificativa').focus();
            return false;
        }
        if (jQuery('#descdesenvolvido').val() == "") {
            alert('O campo O que será desenvolvido é obrigatório.');
            jQuery('#descdesenvolvido').focus();
            return false;
        }
        if (jQuery('#descoqueseradesenv').val() == "") {
            alert('O campo Como será desenvolvido é obrigatório.');
            jQuery('#descoqueseradesenv').focus();
            return false;
        }
        if (jQuery('#desccomunidadeescolar').val() == "") {
            alert('O campo Como a comunidade escolar será integrada ao projeto é obrigatório.');
            jQuery('#desccomunidadeescolar').focus();
            return false;
        }
        if (!jQuery('[name=ptrcomunidadelocal]:checked').val()) {
            alert('O campo Haverá envolvimento da comunidade local é obrigatório.');
            jQuery('[name=ptrcomunidadelocal]').focus();
            return false;
        }

        if (jQuery('[name=ptrcomunidadelocal]:checked').val() == 'S') {
            if (jQuery('[name=desccomunidadelocal]').val() == '') {
                alert('O campo descrição do produto é obrigatório!');
                jQuery('[name=desccomunidadelocal]').focus();
                return false;
            }
        }
        if (jQuery('#descrelprojetoescola').val() == "") {
            alert('O campo Qual a relação entre o projeto proposto e o projeto político pedagógico da escola é obrigatório.');
            jQuery('#descrelprojetoescola').focus();
            return false;
        }
        if (!jQuery('[name=ptrproduto]:checked').val()) {
            alert('O campo Haverá produto é obrigatório.');
            jQuery('[name=ptrproduto]').focus();
            return false;
        }
        if (jQuery('[name=ptrproduto]:checked').val() == 'S') {
            if (jQuery('[name=ptrdescricaproduto]').val() == '') {
                alert('O campo descrição do produto é obrigatório!');
                jQuery('[name=ptrdescricaproduto]').focus();
                return false;
            }
        }
        /*if(jQuery('#descelemtodialogo').val()== ""){
         alert('O campo Elementos do diálogo entre o plano de trabalho proposto e o projeto pedagógico da escola é obrigatório.');
         jQuery('#descelemtodialogo').focus();
         return false;
         }		*/
        if (jQuery('[name=ptrquantidadealuno]').val() == '') {
            alert('O campo Estimativa de Pessoas Envolvidas (Estudantes) é obrigatório.');
            jQuery('[name=ptrquantidadealuno').focus();
            return false;
        }
        if (jQuery('[name=ptrquantidadeprofessor]').val() == '') {
            alert('O campo Estimativa de Pessoas Envolvidas (Professores) é obrigatório.');
            jQuery('[name=ptrquantidadeprofessor]').focus();
            return false;
        }
        if (jQuery('[name=ptrquantidadefamiliar]').val() == '') {
            alert('O campo Estimativa de Pessoas Envolvidas (Familiares) é obrigatório.');
            jQuery('[name=ptrquantidadefamiliar]').focus();
            return false;
        }
        if (jQuery('[name=ptrquantidadepessoa]').val() == '') {
            alert('O campo Estimativa de Pessoas Envolvidas (Pessoas da Comunidade) é obrigatório.');
            jQuery('[name=ptrquantidadepessoa]').focus();
            return false;
        }

        if (jQuery('#ptrresultadosesperados').val() == "") {
            alert('O campo Resultados Esperados é obrigatório.');
            jQuery('#ptrresultadosesperados').focus();
            return false;
        }
        if (jQuery('[name=cradescricao[1]]').val() == '') {
            alert('O campo Cronograma de Ação é obrigatório.');
            jQuery('[name=cradescricao[1]]').focus();
            return false;
        }
        if (jQuery('[name=crames_1[]]:checked').length == 0) {
            alert('Selecione ao menos um Mês.');
            jQuery('[name=crames_1[]]').focus();
            return false;
        }
        if (jQuery('[name=promaterialconsumo]').val() == '') {
            alert('O campo Aquisição de Materiais de consumo é obrigatório.');
            jQuery('[name=promaterialconsumo]').focus();
            return false;
        }
        if (jQuery('[name=procontservcultural]').val() == '') {
            alert('O campo Contratação de serviços culturais necessários às atividades artísticas e pedagógicas é obrigatório.');
            jQuery('[name=procontservcultural]').focus();
            return false;
        }
        if (jQuery('[name=procontservdiverso]').val() == '') {
            alert('O campo Contratação de serviços diversos é obrigatório.');
            jQuery('[name=procontservdiverso]').focus();
            return false;
        }
        if (jQuery('[name=prolocacao]').val() == '') {
            alert('O campo Locação de instrumentos, transporte, equipamentos é obrigatório.');
            jQuery('[name=prolocacao]').focus();
            return false;
        }
        if (jQuery('[name=promaterialpermanente]').val() == '') {
            alert('O campo Aquisição de materiais permanentes e equipamentos é obrigatório.');
            jQuery('[name=promaterialpermanente]').focus();
            return false;
        }
        d.submit();
    }

    function somaPessoas(obj) {
        var d = document.formulario;
        var soma = 0;
        if (d.ptrquantidadealuno.value)
            soma += parseFloat(d.ptrquantidadealuno.value);
        if (d.ptrquantidadeprofessor.value)
            soma += parseFloat(d.ptrquantidadeprofessor.value);
        if (d.ptrquantidadefamiliar.value)
            soma += parseFloat(d.ptrquantidadefamiliar.value);
        if (d.ptrquantidadepessoa.value)
            soma += parseFloat(d.ptrquantidadepessoa.value);
        d.totalenvolvidos.value = soma;
    }

    function atualizaOrc(obj) {
        var d = document.formulario;
        var total = d.totalorcamento.value;
        var saldo = 0;
        var soma = 0;
        total = total.replace(/\./gi, "");
        total = total.replace(/,/gi, ".");

        var promaterialconsumo = d.promaterialconsumo.value;
        promaterialconsumo = promaterialconsumo.replace(/\./gi, "");
        promaterialconsumo = promaterialconsumo.replace(/,/gi, ".");
        if (!promaterialconsumo)
            promaterialconsumo = 0;

        var procontservcultural = d.procontservcultural.value;
        procontservcultural = procontservcultural.replace(/\./gi, "");
        procontservcultural = procontservcultural.replace(/,/gi, ".");
        if (!procontservcultural)
            procontservcultural = 0;

        var procontservdiverso = d.procontservdiverso.value;
        procontservdiverso = procontservdiverso.replace(/\./gi, "");
        procontservdiverso = procontservdiverso.replace(/,/gi, ".");
        if (!procontservdiverso)
            procontservdiverso = 0;

        var prolocacao = d.prolocacao.value;
        prolocacao = prolocacao.replace(/\./gi, "");
        prolocacao = prolocacao.replace(/,/gi, ".");
        if (!prolocacao)
            prolocacao = 0;

        var promaterialpermanente = d.promaterialpermanente.value;
        promaterialpermanente = promaterialpermanente.replace(/\./gi, "");
        promaterialpermanente = promaterialpermanente.replace(/,/gi, ".");
        if (!promaterialpermanente)
            promaterialpermanente = 0;


        soma = parseFloat(promaterialconsumo) + parseFloat(procontservcultural) +
                parseFloat(procontservdiverso) + parseFloat(prolocacao) +
                parseFloat(promaterialpermanente);

        if (soma > total) {
            alert("A soma dos valores da Previsão Orçamentária não pode ser maior que R$ " + d.totalorcamento.value);
            obj.value = '';
            atualizaOrc(obj);
        } else {
            saldo = total - soma;
            d.saldoorcamento.value = mascaraglobal('###.###.###.###,##', saldo.toFixed(2));
        }
    }

</script>
<?php if ($ptrproduto == 't'): ?>
    <script>
        jQuery(function() {
            jQuery('#tr_descricao_produto').show();
        });
    </script>
<?php endif; ?>
<?php if ($ptrcomunidadelocal == 't'): ?>
    <script>
        jQuery(function() {
            jQuery('#tr_envolvimento').show();
        });
    </script>
<?php endif; ?>

<?php if(verificaEncerramentoPlanoEscola()): ?>

	<script>
		jQuery(function(){			
			jQuery.each(jQuery('#formulario')[0].elements, function(i, v){
				jQuery(v).attr('disabled', true).removeClass('normal').addClass('disabled');
			});
			jQuery('#btncancelar, #btngravar, #tr_acoes, #btSalvarC1').remove();
		});
	</script>

<?php endif; ?>