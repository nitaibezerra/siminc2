<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" style="width:100%" >
    <tr>
        <td colspan="2">
            <div style="float:right">
                <img title="Imprimir questionário" onclick="imprimirQuestionario()" src="../imagens/print.png" class="link">
            </div>
        </td>
    </tr>
    <tr>
        <td colspan="2">
            <?php
            switch ($tecnologiaMi->tmiid) {
                case TMIID_PVC_CONCRETO:
                    $qstescopo = 'SMI1';
                    break;
                case TMIID_MATERIAIS_COMPOSITOS:
                    $qstescopo = 'SMI3';
                    break;
                default:
                    $qstescopo = '';
                    break;
            }

            $questao = new Questao();
            $arFiltro = array("qstescopo" => $qstescopo,
                              "orgid" => $_SESSION['obras2']['orgid'],
                              "smiid" => ($smiid ? $smiid : 0));
            $arDados = $questao->pegaTodaEstrutura($arFiltro);

            $etqidUlt = null;
            $itcidUlt = null;
            $qstidUlt = null;

            $subQuestao = new SubQuestao();
            foreach ($arDados as $k => $dados) {

                $arDadosSubQuestao = $subQuestao->pegaSubQuestaoPorQstid($dados['qstid']);

                $qstctrlobs = json_decode($dados['qstctrlobs']);
                switch ($dados['qtsresposta']) {
                    case 't':
                        $obsClass = ($qstctrlobs->S ? 'divQuestaoNivel3' : 'divQuestaoNivel3_none');
                        break;
                    case 'f':
                        $obsClass = ($qstctrlobs->N ? 'divQuestaoNivel3' : 'divQuestaoNivel3_none');
                        break;
                    case 'n':
                        $obsClass = ($qstctrlobs->NA ? 'divQuestaoNivel3' : 'divQuestaoNivel3_none');
                        break;
                    default:
                        $obsClass = 'divQuestaoNivel3_none';
                        break;
                }

                $qstctrlimg = json_decode($dados['qstctrlimagem']);
                switch ($dados['qtsresposta']) {
                    case 't':
                        $imgClass = ($qstctrlimg->S ? 'divQuestaoNivel4' : 'divQuestaoNivel4_none');
                        break;
                    case 'f':
                        $imgClass = ($qstctrlimg->N ? 'divQuestaoNivel4' : 'divQuestaoNivel4_none');
                        break;
                    case 'n':
                        $imgClass = ($qstctrlimg->NA ? 'divQuestaoNivel4' : 'divQuestaoNivel4_none');
                        break;
                    default:
                        $imgClass = 'divQuestaoNivel4_none';
                        break;
                }
                // ABRE Questionário
                if ($etqidUlt != $dados['etqid']) {
                    if (!is_null($etqidUlt)) {
                        $html .= "</fieldset>";
                    }
                    $etqidUlt = $dados['etqid'];
                    $html .= "<fieldset>
					      	<legend>{$dados['etqdsc']}</legend>";
                }
                // ETAPA
                if ($itcidUlt != $dados['itcid']) {
                    $itcidNum++;
                    $html .= "<div class='divItemComposicao'>{$itcidNum} - {$dados['itcdsc']}</div>";
                    $itcidUlt = $dados['itcid'];
                }
                // DIVISÃO
                if ($dvqidUlt != $dados['dvqid'] && !empty($dados['dvqid'])) {
                    $html .= "<div class='divDivisao'>{$dados['dvqnumero']} - {$dados['dvqdsc']}</div>";
                    $dvqidUlt = $dados['dvqid'];
                }
                // QUESTÃO
                if ($qstidUlt != $dados['qstid']) {
                    // Sub Questão
                    $subClass = ($dados['qtsresposta'] == 'f' ? 'divSubQuestao' : 'divSubQuestao_none' );
                    $subItemClass = 'divSubItemQuestao';

                    $subImgClass = "divSubQuestaoNivel4_none";
                    $htmlSubQuestao = "";
                    if (count($arDadosSubQuestao) > 0) {
                        $htmlSubQuestao .= "<div id=\"div_subquestao_{$dados['qstid']}_{$dadosSubQuestao['sqtid']}\" class=\"{$subClass}\">
	    									<div class=\"divSubQuestaoNivel1\">Tipo:</div>";
                    }
                    foreach ($arDadosSubQuestao as $dadosSubQuestao) {
                        $arResultadosDadosSubQuestao = $subQuestao->pegaResultadosSubQuestaoPorSqtidQtsid($dadosSubQuestao['sqtid'], ($dados['qtsid'] ? $dados['qtsid'] : 0));
                        $subObsClass = $arResultadosDadosSubQuestao[0]['rsqstatus'] == 'A' ? "divSubQuestaoNivel3" : "divSubQuestaoNivel3_none";
                        $checked = $arResultadosDadosSubQuestao[0]['rsqstatus'] == 'A' ? "checked=\"checked\"" : '';
                        $restricao = '';
                        if ($dadosSubQuestao['mrqid'] != '') {
                            $restricao = "<input type=\"hidden\" name=\"mrqid[{$dados['qstid']}][{$dadosSubQuestao['sqtid']}]\" value=\"{$dadosSubQuestao['mrqid']}\">";
                        }

                        $htmlSubQuestao .= "<div id=\"div_subquestao_item_{$dadosSubQuestao['sqtid']}\" class=\"{$subItemClass}\">
                                            <div class=\"divSubQuestaoNivel2\">
                                                <input type=\"checkbox\" $checked
                                                           value=\"{$dadosSubQuestao['sqtid']}\" 
                                                           name=\"sqtid[{$dados['qstid']}][]\" 
                                                           id=\"sqtid_{$dadosSubQuestao['sqtid']}\" 
                                                           " . ($habilitado ? '' : 'disabled="disabled"') . "
                                                           onclick=\"ctrlImgObs(this, {$dadosSubQuestao['sqtid']});\"> 
                                                <label for=\"sqtid_{$dadosSubQuestao['sqtid']}\">
                                                {$dadosSubQuestao['sqtdsc']}
                                                </label>
                                            </div>
                                                <div class='" . $subObsClass . "' id='div_obs_sub_{$dadosSubQuestao['sqtid']}'>
                                                    <div class='divSubQuestaoNivel3_1'>Observação:</div>
                                                    $restricao
                                                    <input type=\"hidden\" name=\"rsqid[{$dados['qstid']}][{$dadosSubQuestao['sqtid']}]\" value=\"{$arResultadosDadosSubQuestao[0]['rsqid']}\">
                                                    " . campo_textarea('rsqobs_' . $dados['qstid'] . '_' . $dadosSubQuestao['sqtid'], 'S', $somenteLeitura, '', 100, 4, 1000, '', '', '', '', '', $arResultadosDadosSubQuestao[0]['rsqobs'], array('id' => "qtsobs_sub_{$dadosSubQuestao['sqtid']}")) . "
                                                </div>
                                            </div>";
                        $htmlSubQuestaoTipo = '';
                        // Quando houver subquestão e a resposta for "não" virá fechada a OBSERVAÇÃO
                        if ($dados['qtsresposta'] == 'f') {
                            $obsClass = 'divQuestaoNivel3_none';
                            $dados['qtsobs'] = '';
                        }
                    }
                    if (count($arDadosSubQuestao) > 0) {
                        $htmlSubQuestao .= "</div>";
                    }

                    $html .= "<div class='divQuestao'>
							<div class='divQuestaoNivel1'>{$dados['qstnumero']} - {$dados['qstdsc']}</div>
							<div class='divQuestaoNivel2'>
								<input type='hidden' name='qtsid_{$dados['qstid']}' id='qtsid_{$dados['qstid']}' value='{$dados['qtsid']}'>
								<input 
									name='qstid[{$dados['qstid']}]' 
									id='qstid_t_{$dados['qstid']}' value='t' 
									type='radio' 
									" . ($habilitado ? '' : 'disabled="disabled"') . "
									onclick='ctrlObs({$dados['qstid']}, " . ($qstctrlobs->S ? 'true' : 'false' ) . "); ctrlSubPergunta({$dados['qstid']}, false);' " .
                            ($dados['qtsresposta'] == 't' ? "checked='checked'" : "") . ">
								<label for='qstid_t_{$dados['qstid']}'>Sim</label>
								&nbsp;&nbsp;
								<input 
									name='qstid[{$dados['qstid']}]' 
									id='qstid_f_{$dados['qstid']}' 
									value='f' 
									type='radio'
									" . ($habilitado ? '' : 'disabled="disabled"') . " 
									onclick='ctrlObs({$dados['qstid']}, " . ($qstctrlobs->N ? 'true' : 'false' ) . "); ctrlSubPergunta({$dados['qstid']}, true);' " .
                            ($dados['qtsresposta'] == 'f' ? "checked='checked'" : "") . ">
								<label for='qstid_f_{$dados['qstid']}'>Não</label>
								&nbsp;&nbsp;
								<input 
									name='qstid[{$dados['qstid']}]' 
									id='qstid_n_{$dados['qstid']}' 
									value='n' 
									type='radio'
									" . ($habilitado ? '' : 'disabled="disabled"') . " 
									onclick='ctrlObs({$dados['qstid']}, " . ($qstctrlobs->NA ? 'true' : 'false' ) . "); ctrlSubPergunta({$dados['qstid']}, false);' " .
                            ($dados['qtsresposta'] == 'n' ? "checked='checked'" : "") . ">
								<label for='qstid_n_{$dados['qstid']}'>Não se aplica</label>
							</div>
							" . $htmlSubQuestao . "
							<div class='" . $obsClass . "' id='div_obs_{$dados['qstid']}'>
								<div class='divQuestaoNivel3_1'>Observação:</div>
								" . campo_textarea('qtsobs_' . $dados['qstid'], 'S', $somenteLeitura, '', 100, 4, 1000, '', '', '', '', '', $dados['qtsobs'], array('id' => "qtsobs_{$dados['qstid']}")) . "
							</div>";

                    $html .= "</div>";
                    $qstidUlt = $dados['qstid'];
                }
                echo $html;
                $html = null;
            }
            ?>
        </td>
    </tr>
</table>
