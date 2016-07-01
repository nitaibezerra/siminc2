<?php 
include_once '_funcoes_maismedicos.php';

if($_REQUEST['requisicao'] == 'salvarFicha'){
    salvarFicha($_POST);
    die();
}

if($_SESSION['par']['rqmid']){
	$infor = visualizarInfor($_SESSION['par']['rqmid']);
	extract($infor); 
}

$sus_representante = verificarExisteGestorRepres();
$parceria = verificarParceria();
$adesao = verificarAdesao();
$ficha = recuperaDadosFicha($_SESSION['par']['rqmid']);
extract($ficha);
?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>	
<script language="javascript" type="text/javascript">
	function abrirPergunta(perg){
		jQuery("#perga"+perg).hide();
		jQuery("#pergf"+perg).show();
		jQuery("#div"+perg).show();
	}

	function fecharPergunta(perg){
		jQuery("#perga"+perg).show();
		jQuery("#pergf"+perg).hide();
		jQuery("#div"+perg).hide();
	}

	function imprimir(){
        return window.open('par.php?modulo=principal/programas/feirao_programas/maisMedicosFichaImprimir&acao=A', 'modelo', "height=600,width=950,scrollbars=yes,top=50,left=200");
	}	
</script>

<form action="par.php?modulo=principal/programas/feirao_programas/maisMedicosAnalise&acao=A&aba=Ficha" method="POST" id="formulario" name="formulario" enctype="multipart/form-data">
<input type="hidden" id="requisicao" name="requisicao" value="salvarFicha"/>
<input type="hidden" id="rqmid" name="rqmid" value="<?php echo $_SESSION['par']['rqmid']; ?>"/>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
    <tr>
        <td bgcolor="#CCCCCC"><b>1. PRIMEIRA ETAPA DA PRÉ-SELEÇÃO (ELIMINATÓRIA)</b></td>
    </tr>
    <tr>
        <td>
            <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                <tr>
                    <td class="subtituloEsquerda" width="50%">O município atende aos critérios abaixo:</td>
                </tr>
                <tr>
                    <td class="subtituloEsquerda">
                        <span style="cursor:pointer" onclick="abrirPergunta('01');"><img id="perga01" border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif"></span>
                        <span style="cursor:pointer" onclick="fecharPergunta('01');"><img id="pergf01" border="0" style="display:none;vertical-align:middle;" src="../imagens/gif_inclui_d.gif"></span>	
                        1.1 Ter 70 mil ou mais habitantes
                    </td>
                </tr>	
                <tr>
                    <td id="div01" style="display:none;">
                        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                            <tr>
                                <td class="subtituloEsquerda">
                                    <input id="rqmquestao03" type="radio" name="rqmquestao03" class="normal" style="margin-top: -1px;" <?php echo $rqmquestao03 > 70000 ? 'checked="checked"' : ''; ?> disabled />Sim 
                                    <input id="rqmquestao03" type="radio" name="rqmquestao03" class="normal" style="margin-top: -1px;" <?php echo $rqmquestao03 < 70000 ? 'checked="checked"' : ''; ?> disabled />Não
                                </td>
                            </tr>
                            <tr>
                                <td class="subtituloEsquerda">Observações:</td>	
                            </tr>
                            <tr>
                                <td align="center"><?php echo campo_textarea('rqmobsaval70mil', 'N', 'S', '', '200', '8', '3000', '', '', '', '', '', $rqmobsaval70mil); ?></td>
                            </tr>			    		
                        </table>
                    </td>
                </tr>
                <tr>
                    <td class="subtituloEsquerda">
                        <span style="cursor:pointer" onclick="abrirPergunta('02');"><img id="perga02" border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif"></span>
                        <span style="cursor:pointer" onclick="fecharPergunta('02');"><img id="pergf02" border="0" style="vertical-align:middle;display:none;" src="../imagens/gif_inclui_d.gif"></span>	
                        1.2 Não se constitui capital de estado
                    </td>
                </tr>	
                <tr id="div02" style="display:none;">
                    <td>
                        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">	
                            <tr>
                                <td class="subtituloEsquerda">
                                    <input id="rqmquestao04" type="radio" name="rqmquestao04" class="normal" style="margin-top: -1px;" <?php echo $rqmquestao04 == 't' ? 'checked="checked"' : ''; ?> disabled />Sim
                                    <input id="rqmquestao04" type="radio" name="rqmquestao04" class="normal" style="margin-top: -1px;" <?php echo $rqmquestao04 == 'f' ? 'checked="checked"' : ''; ?> disabled />Não
                                </td>
                            </tr>	
                            <tr>
                                <td class="subtituloEsquerda">Observações:</td>	
                            </tr>
                            <tr>
                                <td align="center"><?php echo campo_textarea('rqmobsavacapitalestado', 'N', 'S', '', '200', '8', '3000', '', '', '', '', '', $rqmobsavacapitalestado); ?></td>
                            </tr>								    
                        </table>
                    </td>
                </tr>			    				    
                <tr>
                    <td class="subtituloEsquerda">
                        <span style="cursor:pointer" onclick="abrirPergunta('03');"><img id="perga03" border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif"></span>
                        <span style="cursor:pointer" onclick="fecharPergunta('03');"><img id="pergf03" border="0" style="vertical-align:middle;display:none;" src="../imagens/gif_inclui_d.gif"></span>	
                        1.3 Não possui curso de medicina em seu território
                    </td>
                </tr>	
                <tr id="div03" style="display:none;">
                    <td>
                        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">				    
                            <tr>
                                <td class="subtituloEsquerda">
                                    <input id="rqmquestao05" type="radio" name="rqmquestao05" class="normal" style="margin-top: -1px;" <?php echo $rqmquestao05 == 't' ? 'checked="checked"' : ''; ?> disabled />Sim
                                    <input id="rqmquestao05" type="radio" name="rqmquestao05" class="normal" style="margin-top: -1px;" <?php echo $rqmquestao05 == 'f' ? 'checked="checked"' : ''; ?> disabled />Não
                                </td>
                            </tr>	
                            <tr>
                                <td class="subtituloEsquerda">Observações:</td>	
                            </tr>
                            <tr>
                                <td align="center"><?php echo campo_textarea('rqmobsavalcursomedicina', 'N', 'S', '', '200', '8', '3000', '', '', '', '', '', $rqmobsavalcursomedicina); ?></td>
                            </tr>					    	
                        </table>
                    </td>
                </tr>							    	    			    
            </table>	
        </td>
    </tr>
    <tr>
        <td bgcolor="#CCCCCC"><b>2. SEGUNDA ETAPA DA PRÉ-ANÁLISE (ELIMINATÓRIA)</b></td>
    </tr>
    <tr>
        <td>
            <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">    
                <tr>
                    <td class="subtituloEsquerda" colspan="3">
                        <span style="cursor:pointer" onclick="abrirPergunta('04');"><img id="perga04" border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif"></span>
                        <span style="cursor:pointer" onclick="fecharPergunta('04');"><img id="pergf04" border="0" style="vertical-align:middle;display:none;" src="../imagens/gif_inclui_d.gif"></span>	
                        2.1 número de leitos disponíveis SUS por aluno maior ou igual a 5 (cinco), ou seja, para um curso com 50 vagas, o município deverá possuir, no mínimo, 250 leitos disponíveis SUS. Obs.: Em caso negativo, o município poderá indicar parceiros, desde que esses municípos tenham, pelo menos, 50 leitos SUS disponíveis.
                    </td>
                </tr>	
                <tr id="div04" style="display:none;">
                    <td>
                        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">				    			    
                            <tr>
                                <td class="subtituloEsquerda">
                                    <input id="rqmquestao06" type="radio" name="rqmquestao06" class="normal" style="margin-top: -1px;" <?php echo $rqmquestao06 > 250 ? 'checked="checked"' : ''; ?> disabled />Sim
                                    <input id="rqmquestao06" type="radio" name="rqmquestao06" class="normal" style="margin-top: -1px;" <?php echo $rqmquestao06 < 250 ? 'checked="checked"' : ''; ?> disabled />Não
                                </td>
                            </tr>	
                            <tr>
                                <td class="subtituloEsquerda">Observações:</td>	
                            </tr>
                            <tr>
                                <td align="center"><?php echo campo_textarea('rqmobsaval250leitos', 'N', 'S', '', '200', '8', '3000', '', '', '', '', '', $rqmobsaval250leitos); ?></td>
                            </tr>							    
                        </table>
                    </td>
                </tr>								    		    
                <tr>
                    <td class="subtituloEsquerda" colspan="3">
                        <span style="cursor:pointer" onclick="abrirPergunta('05');"><img id="perga05" border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif"></span>
                        <span style="cursor:pointer" onclick="fecharPergunta('05');"><img id="pergf05" border="0" style="vertical-align:middle;display:none;" src="../imagens/gif_inclui_d.gif"></span>	
                        2.2 Estabeleceu parcerias?
                    </td>
                </tr>	
                <tr id="div05" style="display:none;">
                    <td>
                        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">				    
                            <tr>
                                <td class="subtituloEsquerda">
                                    <input id="parceria" type="radio" name="parceria" class="normal" style="margin-top: -1px;" <?php echo $parceria == 't' ? 'checked="checked"' : ''; ?> disabled />Sim
                                    <input id="parceria" type="radio" name="parceria" class="normal" style="margin-top: -1px;" <?php echo $parceria == 'f' ? 'checked="checked"' : ''; ?> disabled />Não
                                </td>
                            </tr>
                            <tr>
                                <td class="subtituloEsquerda">Observações:</td>	
                            </tr>
                            <tr>
                                <td align="center"><?php echo campo_textarea('rqmobsavalparceria', 'N', 'S', '', '200', '8', '3000', '', '', '', '', '', $rqmobsavalparceria); ?></td>
                            </tr>							    
                        </table>
                    </td>
                </tr>								    		
                <tr>
                    <td class="subtituloEsquerda" colspan="3">
                        <span style="cursor:pointer" onclick="abrirPergunta('06');"><img id="perga06" border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif"></span>
                        <span style="cursor:pointer" onclick="fecharPergunta('06');"><img id="pergf06" border="0" style="vertical-align:middle;display:none;" src="../imagens/gif_inclui_d.gif"></span>	
                        2.3 Número de alunos por equipe de atenção básica menor ou igual a 3 (três), considerando o mínimo de 17 (dezessete) equipes;
                    </td>
                </tr>
                <tr id="div06" style="display:none;">
                    <td>
                        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">				    					    
                            <tr>
                                <td class="subtituloEsquerda">
                                    <input id="rqmquestao07" type="radio" name="rqmquestao07" class="normal" style="margin-top: -1px;" <?php echo $rqmquestao07 <= 3 ? 'checked="checked"' : ''; ?> disabled />Sim
                                    <input id="rqmquestao07" type="radio" name="rqmquestao07" class="normal" style="margin-top: -1px;" <?php echo $rqmquestao07 > 3 ? 'checked="checked"' : ''; ?> disabled />Não
                                </td>
                            </tr>
                            <tr>
                                <td class="subtituloEsquerda">Observações:</td>	
                            </tr>
                            <tr>
                                <td align="center"><?php echo campo_textarea('rqmobsavalalunoequipbasica', 'N', 'S', '', '200', '8', '3000', '', '', '', '', '', $rqmobsavalalunoequipbasica); ?></td>
                            </tr>							    
                        </table>
                    </td>
                </tr>					    
                <tr>
                    <td class="subtituloEsquerda" colspan="3">
                        <span style="cursor:pointer" onclick="abrirPergunta('07');"><img id="perga07" border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif"></span>
                        <span style="cursor:pointer" onclick="fecharPergunta('07');"><img id="pergf07" border="0" style="vertical-align:middle;display:none;" src="../imagens/gif_inclui_d.gif"></span>	
                        2.4 Existência de leitos de urgência e emergência ou Pronto Socorro;
                    </td>
                </tr>	
                <tr id="div07" style="display:none;">
                    <td>
                        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">				    				    
                            <tr>
                                <td class="subtituloEsquerda">
                                    <input id="rqmquestao08" type="radio" name="rqmquestao08" class="normal" style="margin-top: -1px;" <?php echo $rqmquestao08 == 't' ? 'checked="checked"' : ''; ?> disabled />Sim
                                    <input id="rqmquestao08" type="radio" name="rqmquestao08" class="normal" style="margin-top: -1px;" <?php echo $rqmquestao08 == 'f' ? 'checked="checked"' : ''; ?> disabled />Não
                                </td>
                            </tr>	
                            <tr>
                                <td class="subtituloEsquerda">Observações:</td>	
                            </tr>
                            <tr>
                                <td align="center"><?php echo campo_textarea('rqmobsavalleitourgencia', 'N', 'S', '', '200', '8', '3000', '', '', '', '', '', $rqmobsavalleitourgencia); ?></td>
                            </tr>							    
                        </table>
                    </td>
                </tr>								    			    
                <tr>
                    <td class="subtituloEsquerda" colspan="3">
                        <span style="cursor:pointer" onclick="abrirPergunta('08');"><img id="perga08" border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif"></span>
                        <span style="cursor:pointer" onclick="fecharPergunta('08');"><img id="pergf08" border="0" style="vertical-align:middle;display:none;" src="../imagens/gif_inclui_d.gif"></span>	
                        2.5 Existência de, pelo menos 3 (três), Programas de Residência Médica nas especialidades prioritárias: (1) Clínica Médica; (2) Cirurgia; (3)Ginecologia-Obstetrícia; (4) Pediatria e (5) Medicina de Família e Comunidade;
                    </td>
                </tr>
                <tr id="div08" style="display:none;">
                    <td>
                        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">				    	
                            <tr>
                                <td class="subtituloEsquerda">
                                    <input id="rqmquestao10" type="radio" name="rqmquestao10" class="normal" style="margin-top: -1px;" <?php echo $rqmquestao10 == 't' ? 'checked="checked"' : ''; ?> disabled />Sim
                                    <input id="rqmquestao10" type="radio" name="rqmquestao10" class="normal" style="margin-top: -1px;" <?php echo $rqmquestao10 == 'f' ? 'checked="checked"' : ''; ?> disabled />Não
                                </td>
                            </tr>	
                            <tr>
                                <td class="subtituloEsquerda">Observações:</td>	
                            </tr>
                            <tr>
                                <td align="center"><?php echo campo_textarea('rqmobsavalresidmedica', 'N', 'S', '', '200', '8', '3000', '', '', '', '', '', $rqmobsavalresidmedica); ?></td>
                            </tr>							    
                        </table>
                    </td>
                </tr>								    	
                <tr>
                    <td class="subtituloEsquerda" colspan="3">
                        <span style="cursor:pointer" onclick="abrirPergunta('09');"><img id="perga09" border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif"></span>
                        <span style="cursor:pointer" onclick="fecharPergunta('09');"><img id="pergf09" border="0" style="vertical-align:middle;display:none;" src="../imagens/gif_inclui_d.gif"></span>	
                        2.6 Aderiu ao Termo de Compromisso de Residência?
                    </td>
                </tr>	
                <tr id="div09" style="display:none;">
                    <td>
                        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">				    
                            <tr>
                                <td class="subtituloEsquerda">
                                    <input id="rqmaceitetermoresidencia" type="radio" name="rqmaceitetermoresidencia" class="normal" style="margin-top: -1px;" <?php echo $rqmaceitetermoresidencia == 't' ? 'checked="checked"' : ''; ?> disabled />Sim
                                    <input id="rqmaceitetermoresidencia" type="radio" name="rqmaceitetermoresidencia" class="normal" style="margin-top: -1px;" <?php echo $rqmaceitetermoresidencia == 'f' ? 'checked="checked"' : ''; ?> disabled />Não
                                </td>
                            </tr>
                            <tr>
                                <td class="subtituloEsquerda">Observações:</td>	
                            </tr>
                            <tr>
                                <td align="center"><?php echo campo_textarea('rqmobstermoresidencia', 'N', 'S', '', '200', '8', '3000', '', '', '', '', '', $rqmobstermoresidencia); ?></td>
                            </tr>							    
                        </table>
                    </td>
                </tr>								    
                <tr>
                    <td class="subtituloEsquerda" colspan="3">
                        <span style="cursor:pointer" onclick="abrirPergunta('10');"><img id="perga10" border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif"></span>
                        <span style="cursor:pointer" onclick="fecharPergunta('10');"><img id="pergf10" border="0" style="vertical-align:middle;display:none;" src="../imagens/gif_inclui_d.gif"></span>	
                        2.7 Adesão pelo município ao Programa Nacional de Melhoria do Acesso e da Qualidade na Atenção Básica - PMAQ, do Ministério da Saúde;
                    </td>
                </tr>	
                <tr id="div10" style="display:none;">
                    <td>
                        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">				    	
                            <tr>
                                <td class="subtituloEsquerda">
                                    <input id="rqmquestao11" type="radio" name="rqmquestao11" class="normal" style="margin-top: -1px;" <?php echo $rqmquestao11 == 't' ? 'checked="checked"' : ''; ?> disabled />Sim
                                    <input id="rqmquestao11" type="radio" name="rqmquestao11" class="normal" style="margin-top: -1px;" <?php echo $rqmquestao11 == 'f' ? 'checked="checked"' : ''; ?> disabled />Não
                                </td>
                            </tr>	
                            <tr>
                                <td class="subtituloEsquerda">Observações:</td>	
                            </tr>
                            <tr>
                                <td align="center"><?php echo campo_textarea('rqmobsavalpmaq', 'N', 'S', '', '200', '8', '3000', '', '', '', '', '', $rqmobsavalpmaq); ?></td>
                            </tr>							    
                        </table>
                    </td>
                </tr>								    
                <tr>
                    <td class="subtituloEsquerda" colspan="3">
                        <span style="cursor:pointer" onclick="abrirPergunta('11');"><img id="perga11" border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif"></span>
                        <span style="cursor:pointer" onclick="fecharPergunta('11');"><img id="pergf11" border="0" style="vertical-align:middle;display:none;" src="../imagens/gif_inclui_d.gif"></span>	
                        2.8 Existência de Centro de Atenção Psicossocial - CAPS;
                    </td>
                </tr>	
                <tr id="div11" style="display:none;">
                    <td>
                        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">					    
                            <tr>
                                <td class="subtituloEsquerda">
                                    <input id="rqmquestao12" type="radio" name="rqmquestao12" class="normal" style="margin-top: -1px;" <?php echo $rqmquestao12 == 't' ? 'checked="checked"' : ''; ?> disabled />Sim
                                    <input id="rqmquestao12" type="radio" name="rqmquestao12" class="normal" style="margin-top: -1px;" <?php echo $rqmquestao12 == 'f' ? 'checked="checked"' : ''; ?> disabled />Não
                                </td>
                            </tr>
                            <tr>
                                <td class="subtituloEsquerda">Observações:</td>	
                            </tr>
                            <tr>
                                <td align="center"><?php echo campo_textarea('rqmobsavalcaps', 'N', 'S', '', '200', '8', '3000', '', '', '', '', '', $rqmobsavalcaps); ?></td>
                            </tr>							    
                        </table>
                    </td>	
                </tr>	
                <tr>
                    <td class="subtituloEsquerda" colspan="3">
                        <span style="cursor:pointer" onclick="abrirPergunta('12');"><img id="perga12" border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif"></span>
                        <span style="cursor:pointer" onclick="fecharPergunta('12');"><img id="pergf12" border="0" style="vertical-align:middle;display:none;" src="../imagens/gif_inclui_d.gif"></span>	
                        2.9 Hospital de ensino ou unidade hospitalar com potencial para hospital de ensino, conforme legislação de regência; e
                    </td>
                </tr>	
                <tr id="div12" style="display:none;">
                    <td>
                        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
                            <tr>
                                <td class="subtituloEsquerda">
                                    <input id="rqmquestao13" type="radio" name="rqmquestao13" class="normal" style="margin-top: -1px;" <?php echo $rqmquestao13 == 't' ? 'checked="checked"' : ''; ?> disabled />Sim
                                    <input id="rqmquestao13" type="radio" name="rqmquestao13" class="normal" style="margin-top: -1px;" <?php echo $rqmquestao13 == 'f' ? 'checked="checked"' : ''; ?> disabled />Não
                                </td>
                            </tr>	
                            <tr>
                                <td class="subtituloEsquerda">Observações:</td>	
                            </tr>
                            <tr>
                                <td align="center"><?php echo campo_textarea('rqmobsavalhospensino', 'N', 'S', '', '200', '8', '3000', '', '', '', '', '', $rqmobsavalhospensino); ?></td>
                            </tr>								    
                        </table>
                    </td>	
                </tr>	
                <tr>
                    <td class="subtituloEsquerda">
                        <span style="cursor:pointer" onclick="abrirPergunta('13');"><img id="perga13" border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif"></span>
                        <span style="cursor:pointer" onclick="fecharPergunta('13');"><img id="pergf13" border="0" style="vertical-align:middle;display:none;" src="../imagens/gif_inclui_d.gif"></span>	
                        2.10 Existência de hospital com mais de 100 (cem) leitos exclusivos para o curso.
                    </td>
                </tr>	
                <tr id="div13" style="display:none;">
                    <td>
                        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">					    
                            <tr>
                                <td class="subtituloEsquerda">
                                    <input id="rqmquestao14" type="radio" name="rqmquestao14" class="normal" style="margin-top: -1px;" <?php echo $rqmquestao14 == 't' ? 'checked="checked"' : ''; ?> disabled />Sim
                                    <input id="rqmquestao14" type="radio" name="rqmquestao14" class="normal" style="margin-top: -1px;" <?php echo $rqmquestao14 == 'f' ? 'checked="checked"' : ''; ?> disabled />Não
                                </td>
                            </tr>	
                            <tr>
                                <td class="subtituloEsquerda">Observações:</td>	
                            </tr>
                            <tr>
                                <td align="center"><?php echo campo_textarea('rqmobsaval100leitos', 'N', 'S', '', '200', '8', '3000', '', '', '', '', '', $rqmobsaval100leitos); ?></td>
                            </tr>								    	
                        </table>
                    </td>	
                </tr>	
                <tr>
                    <td class="subtituloEsquerda">
                        <span style="cursor:pointer" onclick="abrirPergunta('14');"><img id="perga14" border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif"></span>
                        <span style="cursor:pointer" onclick="fecharPergunta('14');"><img id="pergf14" border="0" style="vertical-align:middle;display:none;" src="../imagens/gif_inclui_d.gif"></span>	
                        2.11 Indicou o gestor local do sus e o representante legal?
                    </td>
                </tr>		
                <tr id="div14" style="display:none;">
                    <td>
                        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">					    			    		    
                            <tr>
                                <td class="subtituloEsquerda">
                                    <input id="sus_representante" type="radio" name="sus_representante" class="normal" style="margin-top: -1px;" <?php echo $sus_representante == 't' ? 'checked="checked"' : ''; ?> disabled />Sim
                                    <input id="sus_representante" type="radio" name="sus_representante" class="normal" style="margin-top: -1px;" <?php echo $sus_representante == 'f' ? 'checked="checked"' : ''; ?> disabled />Não
                                </td>
                            </tr>	
                            <tr>
                                <td class="subtituloEsquerda">Observações:</td>	
                            </tr>
                            <tr>
                                <td align="center"><?php echo campo_textarea('rqmobsavalgestorlocal', 'N', 'S', '', '200', '8', '3000', '', '', '', '', '', $rqmobsavalgestorlocal); ?></td>
                            </tr>								    
                        </table>
                    </td>				    
                </tr>
                <tr>
                    <td class="subtituloEsquerda">
                        <span style="cursor:pointer" onclick="abrirPergunta('15');"><img id="perga15" border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif"></span>
                        <span style="cursor:pointer" onclick="fecharPergunta('15');"><img id="pergf15" border="0" style="vertical-align:middle;display:none;" src="../imagens/gif_inclui_d.gif"></span>	
                        2.12 Aderiu ao Termo de Adesão de municípios?
                    </td>
                </tr>			
                <tr id="div15" style="display:none;">
                    <td>
                        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">					    		    				    
                            <tr>
                                <td class="subtituloEsquerda">
                                    <input id="adesao" type="radio" name="adesao" class="normal" style="margin-top: -1px;" <?php echo $adesao == 'S' ? 'checked="checked"' : ''; ?> disabled />Sim
                                    <input id="adesao" type="radio" name="adesao" class="normal" style="margin-top: -1px;" <?php echo $adesao == 'N' ? 'checked="checked"' : ''; ?> disabled />Não
                                </td>
                            </tr>	
                            <tr>
                                <td class="subtituloEsquerda">Observações:</td>	
                            </tr>
                            <tr>
                                <td align="center"><?php echo campo_textarea('rqmobsavaltermomunic', 'N', 'S', '', '200', '8', '3000', '', '', '', '', '', $rqmobsavaltermomunic); ?></td>
                            </tr>								    
                        </table>
                    </td>
                </tr>
                <tr>
                    <td class="subtituloEsquerda">
                        <span style="cursor:pointer" onclick="abrirPergunta('16');"><img id="perga16" border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif"></span>
                        <span style="cursor:pointer" onclick="fecharPergunta('16');"><img id="pergf16" border="0" style="vertical-align:middle;display:none;" src="../imagens/gif_inclui_d.gif"></span>	
                        2.13 Apresentou a documentação comprobatória?
                    </td>
                </tr>	
                <tr id="div16" style="display:none;">
                    <td>
                        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">					    				    
                            <tr>
                                <td class="subtituloEsquerda">
                                    <input id="rqmsitdoccomprobatoria" onclick="abrirPergunta('0');" type="radio" name="rqmsitdoccomprobatoria" value="t" class="normal" style="margin-top: -1px;" <?php echo $rqmsitdoccomprobatoria == 't' ? 'checked="checked"' : ''; ?>/>Sim
                                    <input id="rqmsitdoccomprobatoria" onclick="fecharPergunta('0');" type="radio" name="rqmsitdoccomprobatoria" value="f" class="normal" style="margin-top: -1px;" <?php echo $rqmsitdoccomprobatoria == 'f' ? 'checked="checked"' : ''; ?>/>Não
                                </td>
                            </tr>
                        </table>	
                    </td>
                </tr>		
                <tr id="div0" style="display:none;">
                    <td>	   
                        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
                            <tr>
                                <td class="subtituloEsquerda">
                                    <span style="cursor:pointer" onclick="abrirPergunta('17');"><img id="perga17" border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif"></span>
                                    <span style="cursor:pointer" onclick="fecharPergunta('17');"><img id="pergf17" border="0" style="vertical-align:middle;display:none;" src="../imagens/gif_inclui_d.gif"></span>	
                                    a) oficio de apresentaçao apresentado assinado pelo dirigente municipal
                                </td>
                            </tr>	
                            <tr id="div17" style="display:none;">
                                <td>
                                    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">					    				    
                                        <tr>
                                            <td class="subtituloEsquerda">
                                                <input id="rqmoficiodirigente" type="radio" name="rqmoficiodirigente" value="t" class="normal" style="margin-top: -1px;" <?php echo $rqmoficiodirigente == 't' ? 'checked="checked"' : ''; ?>/>Sim
                                                <input id="rqmoficiodirigente" type="radio" name="rqmoficiodirigente" value="f" class="normal" style="margin-top: -1px;" <?php echo $rqmoficiodirigente == 'f' ? 'checked="checked"' : ''; ?>/>Não
                                            </td>
                                        </tr>
                                        <tr>
                                            <td class="subtituloEsquerda">Observações:</td>	
                                        </tr>
                                        <tr>
                                            <td align="center"><?php echo campo_textarea('rqmobsavaldocoficio', 'N', 'S', '', '200', '8', '3000', '', '', '', '', '', $rqmobsavaldocoficio); ?></td>
                                        </tr>								    
                                    </table>	
                                </td>
                            </tr>
                            <tr>
                                <td class="subtituloEsquerda">
                                    <span style="cursor:pointer" onclick="abrirPergunta('18');"><img id="perga18" border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif"></span>
                                    <span style="cursor:pointer" onclick="fecharPergunta('18');"><img id="pergf18" border="0" style="vertical-align:middle;display:none;" src="../imagens/gif_inclui_d.gif"></span>	
                                    b) cópia do documento de RG e CPF do dirigente municipal e do gestor local do SUS
                                </td>
                            </tr>	
                            <tr id="div18" style="display:none;">
                                <td>
                                    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">					    	
                                        <tr>
                                            <td class="subtituloEsquerda">
                                                <input id="rqmcopiargcpfdir" type="radio" name="rqmcopiargcpfdir" value="t" class="normal" style="margin-top: -1px;" <?php echo $rqmcopiargcpfdir == 't' ? 'checked="checked"' : ''; ?>/>Sim
                                                <input id="rqmcopiargcpfdir" type="radio" name="rqmcopiargcpfdir" value="f" class="normal" style="margin-top: -1px;" <?php echo $rqmcopiargcpfdir == 'f' ? 'checked="checked"' : ''; ?>/>Não
                                            </td>
                                        </tr>
                                        <tr>
                                            <td class="subtituloEsquerda">Observações:</td>	
                                        </tr>
                                        <tr>
                                            <td align="center"><?php echo campo_textarea('rqmobsavaldocrgcpf', 'N', 'S', '', '200', '8', '3000', '', '', '', '', '', $rqmobsavaldocrgcpf); ?></td>
                                        </tr>						    									    
                                    </table>
                                </td>
                            </tr>
                            <tr>
                                <td class="subtituloEsquerda">
                                    <span style="cursor:pointer" onclick="abrirPergunta('19');"><img id="perga19" border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif"></span>
                                    <span style="cursor:pointer" onclick="fecharPergunta('19');"><img id="pergf19" border="0" style="vertical-align:middle;display:none;" src="../imagens/gif_inclui_d.gif"></span>	
                                    c) Projeto de melhorias da estrutura de equipamentos e programas de saúde existentes no município
                                </td>
                            </tr>	
                            <tr id="div19" style="display:none;">
                                <td>
                                    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">					    
                                        <tr>
                                            <td class="subtituloEsquerda">
                                                <input id="rqmprojetomelhoria" type="radio" name="rqmprojetomelhoria" value="t" class="normal" style="margin-top: -1px;" <?php echo $rqmprojetomelhoria == 't' ? 'checked="checked"' : ''; ?>/>Sim
                                                <input id="rqmprojetomelhoria" type="radio" name="rqmprojetomelhoria" value="f" class="normal" style="margin-top: -1px;" <?php echo $rqmprojetomelhoria == 'f' ? 'checked="checked"' : ''; ?>/>Não
                                            </td>
                                        </tr>	
                                        <tr>
                                            <td class="subtituloEsquerda">Observações:</td>	
                                        </tr>
                                        <tr>
                                            <td align="center"><?php echo campo_textarea('rqmobsavalprojetomelhoria', 'N', 'S', '', '200', '8', '3000', '', '', '', '', '', $rqmobsavalprojetomelhoria); ?></td>
                                        </tr>				    							    							    				    				    
                                    </table>
                                </td>
                            </tr>
                            <tr>
                                <td class="subtituloEsquerda">
                                    <span style="cursor:pointer" onclick="abrirPergunta('20');"><img id="perga20" border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif"></span>
                                    <span style="cursor:pointer" onclick="fecharPergunta('20');"><img id="pergf20" border="0" style="vertical-align:middle;display:none;" src="../imagens/gif_inclui_d.gif"></span>	
                                    d) documentos comprobatórios dos itens 3.1  e 3.3 do Edital
                                </td>
                            </tr>	
                            <tr id="div20" style="display:none;">
                                <td>
                                    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">					    
                                        <tr>
                                            <td class="subtituloEsquerda">
                                                <input id="rqmdocitem3133" type="radio" name="rqmdocitem3133" value="t" class="normal" style="margin-top: -1px;" <?php echo $rqmdocitem3133 == 't' ? 'checked="checked"' : ''; ?>/>Sim
                                                <input id="rqmdocitem3133" type="radio" name="rqmdocitem3133" value="f" class="normal" style="margin-top: -1px;" <?php echo $rqmdocitem3133 == 'f' ? 'checked="checked"' : ''; ?>/>Não
                                            </td>
                                        </tr>
                                        <tr>
                                            <td class="subtituloEsquerda">Observações:</td>	
                                        </tr>
                                        <tr>
                                            <td align="center"><?php echo campo_textarea('rqmobsavalitens3133', 'N', 'S', '', '200', '8', '3000', '', '', '', '', '', $rqmobsavalitens3133); ?></td>
                                        </tr>					    			
                                    </table>
                                </td>
                            </tr>	
                            <tr>
                                <td class="subtituloEsquerda">
                                    <span style="cursor:pointer" onclick="abrirPergunta('21');"><img id="perga21" border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif"></span>
                                    <span style="cursor:pointer" onclick="fecharPergunta('21');"><img id="pergf21" border="0" style="vertical-align:middle;display:none;" src="../imagens/gif_inclui_d.gif"></span>	
                                    e) documentos comprobatórios dos itens 4.1, 4.2 e 4.3 do Edital
                                </td>
                            </tr>				    
                            <tr id="div21" style="display:none;">
                                <td>
                                    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
                                        <tr>
                                            <td class="subtituloEsquerda">
                                                <input id="rqmdocitem414243" type="radio" name="rqmdocitem414243" value="t" class="normal" style="margin-top: -1px;" <?php echo $rqmdocitem414243 == 't' ? 'checked="checked"' : ''; ?>/>Sim
                                                <input id="rqmdocitem414243" type="radio" name="rqmdocitem414243" value="f" class="normal" style="margin-top: -1px;" <?php echo $rqmdocitem414243 == 'f' ? 'checked="checked"' : ''; ?>/>Não
                                            </td>
                                        </tr>	
                                        <tr>
                                            <td align="center"><?php echo campo_textarea('rqmobsaval414243', 'N', 'S', '', '200', '8', '3000', '', '', '', '', '', $rqmobsaval414243); ?></td>
                                        </tr>					    			
                                    </table>
                                </td>
                            </tr>
                        </table>
                    </td>
                </tr>
                <tr>
                    <td class="subtituloEsquerda">
                        <span style="cursor:pointer" onclick="abrirPergunta('22');"><img id="perga22" border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif"></span>
                        <span style="cursor:pointer" onclick="fecharPergunta('22');"><img id="pergf22" border="0" style="vertical-align:middle;display:none;" src="../imagens/gif_inclui_d.gif"></span>	
                        2.14 Síntese da avaliação:
                    </td>
                </tr>	
                <tr id="div22" style="display:none;">
                    <td>
                        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">					    				    
                            <tr>
                                <td align="center"><?php echo campo_textarea('rqmsintesesegundaetapa', 'N', 'S', '', '200', '8', '3000', '', '', '', '', '', $rqmsintesesegundaetapa); ?></td>
                            </tr>		
                        </table>	
                    </td>
                </tr>	
            </table>
        </td>
    </tr>	
    <tr>
        <td bgcolor="#CCCCCC"><b>3. TERCEIRA ETAPA DA PRÉ-ANÁLISE (ELIMINATÓRIA)</b></td>
    </tr>
    <tr>
        <td>    
            <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                <tr>
                    <td class="subtituloEsquerda">O projeto de melhorias atende aos objetivos do edital nº 03, quanto à:</td>
                </tr>		
                <tr>
                    <td class="subtituloEsquerda">
                        <span style="cursor:pointer" onclick="abrirPergunta('23');"><img id="perga23" border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif"></span>
                        <span style="cursor:pointer" onclick="fecharPergunta('23');"><img id="pergf23" border="0" style="vertical-align:middle;display:none;" src="../imagens/gif_inclui_d.gif"></span>	
                        3.1 Ampliação de equipes de atenção básica (JUSTIFIQUE)
                    </td>
                </tr>		
                <tr id="div23" style="display:none;">
                    <td>
                        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">					    		    			    
                            <tr>
                                <td class="subtituloEsquerda">
                                    <input id="rqmampliacaoequipebasica" type="radio" name="rqmampliacaoequipebasica" value="S" class="normal" style="margin-top: -1px;" <?php echo $rqmampliacaoequipebasica == 'S' ? 'checked="checked"' : ''; ?>/>Satisfatoriamente
                                    <input id="rqmampliacaoequipebasica" type="radio" name="rqmampliacaoequipebasica" value="P" class="normal" style="margin-top: -1px;" <?php echo $rqmampliacaoequipebasica == 'P' ? 'checked="checked"' : ''; ?>/>Parcialmente satisfatorio
                                    <input id="rqmampliacaoequipebasica" type="radio" name="rqmampliacaoequipebasica" value="I" class="normal" style="margin-top: -1px;" <?php echo $rqmampliacaoequipebasica == 'I' ? 'checked="checked"' : ''; ?>/>Insatisfatório
                                </td>
                            </tr>	
                            <tr>
                                <td class="subtituloEsquerda">Observações:</td>	
                            </tr>
                            <tr>
                                <td align="center"><?php echo campo_textarea('rqmobsampliacaoequipebasica', 'N', 'S', '', '200', '8', '3000', '', '', '', '', '', $rqmobsampliacaoequipebasica); ?></td>
                            </tr>		
                        </table>
                    </td>
                </tr>	
                <tr>
                    <td class="subtituloEsquerda">
                        <span style="cursor:pointer" onclick="abrirPergunta('24');"><img id="perga24" border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif"></span>
                        <span style="cursor:pointer" onclick="fecharPergunta('24');"><img id="pergf24" border="0" style="vertical-align:middle;display:none;" src="../imagens/gif_inclui_d.gif"></span>
                        3.2 Ampliação de leitos disponíveis SUS (especificar o tipo de leito) (JUSTIFIQUE)
                    </td>
                </tr>	
                <tr id="div24" style="display:none;">
                    <td>
                        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">					    
                            <tr>
                                <td class="subtituloEsquerda">
                                    <input id="rqmampliacaoleitos" type="radio" name="rqmampliacaoleitos" value="S" class="normal" style="margin-top: -1px;" <?php echo $rqmampliacaoleitos == 'S' ? 'checked="checked"' : ''; ?>/>Satisfatoriamente
                                    <input id="rqmampliacaoleitos" type="radio" name="rqmampliacaoleitos" value="P" class="normal" style="margin-top: -1px;" <?php echo $rqmampliacaoleitos == 'P' ? 'checked="checked"' : ''; ?>/>Parcialmente satisfatorio
                                    <input id="rqmampliacaoleitos" type="radio" name="rqmampliacaoleitos" value="I" class="normal" style="margin-top: -1px;" <?php echo $rqmampliacaoleitos == 'I' ? 'checked="checked"' : ''; ?>/>Insatisfatório
                                </td>
                            </tr>	
                            <tr>
                                <td class="subtituloEsquerda">Observações:</td>	
                            </tr>
                            <tr>
                                <td align="center"><?php echo campo_textarea('rqmobsampliacaoleitos', 'N', 'S', '', '200', '8', '3000', '', '', '', '', '', $rqmobsampliacaoleitos); ?></td>
                            </tr>		
                        </table>
                    </td>
                </tr>
                <tr>
                    <td class="subtituloEsquerda">
                        <span style="cursor:pointer" onclick="abrirPergunta('25');"><img id="perga25" border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif"></span>
                        <span style="cursor:pointer" onclick="fecharPergunta('25');"><img id="pergf25" border="0" style="vertical-align:middle;display:none;" src="../imagens/gif_inclui_d.gif"></span>	
                        3.3 Criação/ampliação de leitos de urgência e emergência (JUSTIFIQUE)
                    </td>
                </tr>	
                <tr id="div25" style="display:none;">
                    <td>
                        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">					    
                            <tr>
                                <td class="subtituloEsquerda">
                                    <input id="rqmleitosurgencia" type="radio" name="rqmleitosurgencia" value="S" class="normal" style="margin-top: -1px;" <?php echo $rqmleitosurgencia == 'S' ? 'checked="checked"' : ''; ?>/>Satisfatoriamente
                                    <input id="rqmleitosurgencia" type="radio" name="rqmleitosurgencia" value="P" class="normal" style="margin-top: -1px;" <?php echo $rqmleitosurgencia == 'P' ? 'checked="checked"' : ''; ?>/>Parcialmente satisfatorio
                                    <input id="rqmleitosurgencia" type="radio" name="rqmleitosurgencia" value="I" class="normal" style="margin-top: -1px;" <?php echo $rqmleitosurgencia == 'I' ? 'checked="checked"' : ''; ?>/>Insatisfatório
                                </td>
                            </tr>	
                            <tr>
                                <td class="subtituloEsquerda">Observações:</td>	
                            </tr>
                            <tr>
                                <td align="center"><?php echo campo_textarea('rqmobsleitosurgencia', 'N', 'S', '', '200', '8', '3000', '', '', '', '', '', $rqmobsleitosurgencia); ?></td>
                            </tr>	
                        </table>
                    </td>
                </tr>
                <tr>
                    <td class="subtituloEsquerda">
                        <span style="cursor:pointer" onclick="abrirPergunta('26');"><img id="perga26" border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif"></span>
                        <span style="cursor:pointer" onclick="fecharPergunta('26');"><img id="pergf26" border="0" style="vertical-align:middle;display:none;" src="../imagens/gif_inclui_d.gif"></span>	
                        3.4 Hospital de ensino ou unidade com potencial de hospital de ensino (JUSTIFIQUE)
                    </td>
                </tr>	
                <tr id="div26" style="display:none;">
                    <td>
                        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">					    
                            <tr>
                                <td class="subtituloEsquerda">
                                    <input id="rqmhospitalensino" type="radio" name="rqmhospitalensino" value="S" class="normal" style="margin-top: -1px;" <?php echo $rqmhospitalensino == 'S' ? 'checked="checked"' : ''; ?>/>Satisfatoriamente
                                    <input id="rqmhospitalensino" type="radio" name="rqmhospitalensino" value="P" class="normal" style="margin-top: -1px;" <?php echo $rqmhospitalensino == 'P' ? 'checked="checked"' : ''; ?>/>Parcialmente satisfatorio
                                    <input id="rqmhospitalensino" type="radio" name="rqmhospitalensino" value="I" class="normal" style="margin-top: -1px;" <?php echo $rqmhospitalensino == 'I' ? 'checked="checked"' : ''; ?>/>Insatisfatório
                                </td>
                            </tr>	
                            <tr>
                                <td class="subtituloEsquerda">Observações:</td>	
                            </tr>
                            <tr>
                                <td align="center"><?php echo campo_textarea('rqmobshospitalensino', 'N', 'S', '', '200', '8', '3000', '', '', '', '', '', $rqmobshospitalensino); ?></td>
                            </tr>		
                        </table>
                    </td>
                </tr>
                <tr>
                    <td class="subtituloEsquerda">
                        <span style="cursor:pointer" onclick="abrirPergunta('27');"><img id="perga27" border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif"></span>
                        <span style="cursor:pointer" onclick="fecharPergunta('27');"><img id="pergf27" border="0" style="vertical-align:middle;display:none;" src="../imagens/gif_inclui_d.gif"></span>	
                        3.5 Construção/Ampliação de hospital com leitos exclusivos para o curso (JUSTIFIQUE)
                    </td>
                </tr>	
                <tr id="div27" style="display:none;">
                    <td>
                        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">					    
                            <tr>
                                <td class="subtituloEsquerda">
                                    <input id="rqmhospitalleitoscurso" type="radio" name="rqmhospitalleitoscurso" value="S" class="normal" style="margin-top: -1px;" <?php echo $rqmhospitalleitoscurso == 'S' ? 'checked="checked"' : ''; ?>/>Satisfatoriamente
                                    <input id="rqmhospitalleitoscurso" type="radio" name="rqmhospitalleitoscurso" value="P" class="normal" style="margin-top: -1px;" <?php echo $rqmhospitalleitoscurso == 'P' ? 'checked="checked"' : ''; ?>/>Parcialmente satisfatorio
                                    <input id="rqmhospitalleitoscurso" type="radio" name="rqmhospitalleitoscurso" value="I" class="normal" style="margin-top: -1px;" <?php echo $rqmhospitalleitoscurso == 'I' ? 'checked="checked"' : ''; ?>/>Insatisfatório
                                </td>
                            </tr>	
                            <tr>
                                <td class="subtituloEsquerda">Observações:</td>	
                            </tr>
                            <tr>
                                <td align="center"><?php echo campo_textarea('rqmobshospitalleitoscurso', 'N', 'S', '', '200', '8', '3000', '', '', '', '', '', $rqmobshospitalleitoscurso); ?></td>
                            </tr>	
                        </table>
                    </td>				    			    
                </tr>
                <tr>
                    <td class="subtituloEsquerda">
                        <span style="cursor:pointer" onclick="abrirPergunta('28');"><img id="perga28" border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif"></span>
                        <span style="cursor:pointer" onclick="fecharPergunta('28');"><img id="pergf28" border="0" style="vertical-align:middle;display:none;" src="../imagens/gif_inclui_d.gif"></span>	
                        3.6 Criação/ampliação de programas de saúde (JUSTIFIQUE)
                    </td>
                </tr>	
                <tr id="div28" style="display:none;">
                    <td>
                        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">					    
                            <tr>
                                <td class="subtituloEsquerda">
                                    <input id="rqmampliacaoprogsaude" type="radio" name="rqmampliacaoprogsaude" value="S" class="normal" style="margin-top: -1px;" <?php echo $rqmampliacaoprogsaude == 'S' ? 'checked="checked"' : ''; ?>/>Satisfatoriamente
                                    <input id="rqmampliacaoprogsaude" type="radio" name="rqmampliacaoprogsaude" value="P" class="normal" style="margin-top: -1px;" <?php echo $rqmampliacaoprogsaude == 'P' ? 'checked="checked"' : ''; ?>/>Parcialmente satisfatorio
                                    <input id="rqmampliacaoprogsaude" type="radio" name="rqmampliacaoprogsaude" value="I" class="normal" style="margin-top: -1px;" <?php echo $rqmampliacaoprogsaude == 'I' ? 'checked="checked"' : ''; ?>/>Insatisfatório
                                </td>
                            </tr>	
                            <tr>
                                <td class="subtituloEsquerda">Observações:</td>	
                            </tr>
                            <tr>
                                <td align="center"><?php echo campo_textarea('rqmobsampliacaoprogsaude', 'N', 'S', '', '200', '8', '3000', '', '', '', '', '', $rqmobsampliacaoprogsaude); ?></td>
                            </tr>	
                        </table>
                    </td>				    			    
                </tr>
                <tr>
                    <td class="subtituloEsquerda">
                        <span style="cursor:pointer" onclick="abrirPergunta('29');"><img id="perga29" border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif"></span>
                        <span style="cursor:pointer" onclick="fecharPergunta('29');"><img id="pergf29" border="0" style="vertical-align:middle;display:none;" src="../imagens/gif_inclui_d.gif"></span>	
                        3.7 Outras informações (JUSTIFIQUE)
                    </td>
                </tr>
                <tr id="div29" style="display:none;">
                    <td>
                        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">					    	
                            <tr>
                                <td align="center"><?php echo campo_textarea('rqmoutrainfo', 'N', 'S', '', '200', '8', '3000', '', '', '', '', '', $rqmoutrainfo); ?></td>
                            </tr>		
                        </table>
                    </td>				    			    
                </tr>
                <tr>
                    <td class="subtituloEsquerda">
                        <span style="cursor:pointer" onclick="abrirPergunta('30');"><img id="perga30" border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif"></span>
                        <span style="cursor:pointer" onclick="fecharPergunta('30');"><img id="pergf30" border="0" style="vertical-align:middle;display:none;" src="../imagens/gif_inclui_d.gif"></span>	
                        3.8 Síntese da avaliação e sugestão de deferimento ou indeferimento
                    </td>
                </tr>		
                <tr id="div30" style="display:none;">
                    <td>
                        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">					    	
                            <tr>
                                <td align="center"><?php echo campo_textarea('rqmsinteseterceiraetapa', 'N', 'S', '', '200', '8', '3000', '', '', '', '', '', $rqmsinteseterceiraetapa); ?></td>
                            </tr>		
                        </table>
                    </td>				    			    
                </tr>			    		
            </table>
        </td>
    </tr>
    <tr>
        <td align="center" bgcolor="#CCCCCC" colspan="2">
            <?  if( in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) ){ ?>
                    <input type="submit" value="Salvar" id="btnSalvar"/>
            <?  }else{ ?>
                    <input type="button" value="Salvar" id="salvar" disabled="disabled"/>
            <?  } ?>
            &nbsp;&nbsp;&nbsp;
            <input type="button"" value="Imprimir" id="btnImprimir"  onclick="imprimir();";/>
        </td>
    </tr>    
</table>
</form>
<script language="javascript" type="text/javascript">
	<?php if($rqmsitdoccomprobatoria == 't'){ ?>
	jQuery("#div16").show();
	jQuery("#div0").show();
	<?php } ?>
</script>