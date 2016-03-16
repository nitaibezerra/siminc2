<?PHP
    include_once '_funcoes_maismedicos_2015.php';

    if($_REQUEST['requisicao']){
        $_REQUEST['requisicao']($_REQUEST);
    }

    include_once APPRAIZ . "includes/workflow.php";
    include_once APPRAIZ . 'includes/cabecalho.inc';
    echo '<br>';
    echo carregaAbasProFuncionario('par.php?modulo=principal/programas/feirao_programas/mais_medicos_2015/dados_representantes&acao=A');
    monta_titulo("Mais Médicos - Edital nº 1/2015", $titulo_modulo.' <br> '.recuperaMunicipioEstado());


    $prefeito = buscaDadosPrefeito();
    $dados_getor_s = buscaDadosGestorMaisMedicos_novo_2015('S');
    $dados_getor_m = buscaDadosGestorMaisMedicos_novo_2015('M');
    $arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);

    if($_SESSION['par']['rqmid']){
        $docid = pgMaisMedicosCriarDocumento($_SESSION['par']['rqmid']);
    }

    $habilitado = verificaDataFechamento();
    
?>

<table class="tabela" height="100%" align="center" cellspacing="0" cellpadding="3" >
    <tr class="subtituloEsquerda">
        <td height="10" width="45%">
            <input type="button" value="Voltar" onclick="window.location.href='?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';" id="btnVoltar" />
        </td>
    </tr>
</table>

<br>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link href="../includes/jquery-autocomplete/jquery.autocomplete.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script	type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>

<script type="text/javascript">
    jQuery(document).ready(function(){

        jQuery('#salvarDadosMaisMedicos').click(function(){
            var dmmnome_s           = $('#dmmnome_s');
            var dmmcpf_s            = $('#dmmcpf_s');
            var dmmrg_s             = $('#dmmrg_s');
            var dmmsexo_s           = $('input:radio[name=dmmsexo_s]:checked');
            var dmmdtnascimento_s   = $('#dmmdtnascimento_s');
            var estuf_s             = $('#estuf_s');
            var muncodend_s         = $('#muncodend_s');
            var dmmorgao_s          = $('#dmmorgao_s');
            var dmmfonecomercial_s  = $('#dmmfonecomercial_s');
            var dmmcelular_s        = $('#dmmcelular_s');
            var dmmemail_s          = $('#dmmemail_s');
            var dmmcargofuncao_s    = $('#dmmcargofuncao_s');
            var dmmnome_m           = $('#dmmnome_m');
            var dmmcpf_m            = $('#dmmcpf_m');
            var dmmrg_m             = $('#dmmrg_m');
            var dmmsexo_m           = $('input:radio[name=dmmsexo_m]:checked');
            var dmmdtnascimento_m   = $('#dmmdtnascimento_m');
            var estuf_m             = $('#estuf_m');
            var muncodend_m         = $('#muncodend_m');
            var dmmorgao_m          = $('#dmmorgao_m');
            var dmmfonecomercial_m  = $('#dmmfonecomercial_m');
            var dmmcelular_m        = $('#dmmcelular_m');
            var dmmemail_m          = $('#dmmemail_m');
            var dmmcargofuncao_m    = $('#dmmcargofuncao_m');

            var erro;

            //Getor SUS
            if( !dmmnome_s.val() ){
                alert('O campo "Gestor SUS - Nome Completo" é um campo obrigatório!');
                dmmnome_s.focus();
                erro = 1;
                return false;
            }
            if( !dmmcpf_s.val() ){
                alert('O campo "Gestor SUS - CPF" é um campo obrigatório!');
                dmmcpf_s.focus();
                erro = 1;
                return false;
            }
            if( !dmmrg_s.val() ){
                alert('O campo "Gestor SUS - RG" é um campo obrigatório!');
                dmmrg_s.focus();
                erro = 1;
                return false;
            }
            if( !dmmsexo_s.val() ){
                alert('O campo "Gestor SUS - Sexo" é um campo obrigatório!');
                dmmsexo_s.focus();
                erro = 1;
                return false;
            }
            if( !dmmdtnascimento_s.val() ){
                alert('O campo "Gestor SUS - Data de Nascimento" é um campo obrigatório!');
                dmmdtnascimento_s.focus();
                erro = 1;
                return false;
            }
            if( !estuf_s.val() ){
                alert('O campo "Gestor SUS - UF" é um campo obrigatório!');
                estuf_s.focus();
                erro = 1;
                return false;
            }
            if( !muncodend_s.val() ){
                alert('O campo "Gestor SUS - Município" é um campo obrigatório!');
                muncodend_s.focus();
                erro = 1;
                return false;
            }
            if( !dmmorgao_s.val() ){
                alert('O campo "Gestor SUS - Órgão" é um campo obrigatório!');
                dmmorgao_s.focus();
                erro = 1;
                return false;
            }
            if( !dmmfonecomercial_s.val() ){
                alert('O campo "Telefone Fixo Comercial" é um campo obrigatório!');
                dmmfonecomercial_s.focus();
                erro = 1;
                return false;
            }
            if( !dmmcelular_s.val() ){
                alert('O campo "Gestor SUS - Telefone Celular" é um campo obrigatório!');
                dmmcelular_s.focus();
                erro = 1;
                return false;
            }
            if( !dmmemail_s.val() ){
                alert('O campo "Gestor SUS - E-mail" é um campo obrigatório!');
                dmmemail_s.focus();
                erro = 1;
                return false;
            }
            if( !dmmcargofuncao_s.val() ){
                alert('O campo "Gestor SUS - Cargo/Função" é um campo obrigatório!');
                dmmcargofuncao_s.focus();
                erro = 1;
                return false;
            }

            //Getor Local
            if( !dmmnome_m.val() ){
                alert('O campo "Getor Local - Nome Completo" é um campo obrigatório!');
                dmmnome_m.focus();
                erro = 1;
                return false;
            }
            if( !dmmcpf_m.val() ){
                alert('O campo "Getor Local - CPF" é um campo obrigatório!');
                dmmcpf_m.focus();
                erro = 1;
                return false;
            }
            if( !dmmrg_m.val() ){
                alert('O campo "Getor Local - RG" é um campo obrigatório!');
                dmmrg_m.focus();
                erro = 1;
                return false;
            }
            if( !dmmsexo_m.val() ){
                alert('O campo "Getor Local - Sexo" é um campo obrigatório!');
                dmmsexo_m.focus();
                erro = 1;
                return false;
            }
            if( !dmmdtnascimento_m.val() ){
                alert('O campo "Getor Local - Data de Nascimento" é um campo obrigatório!');
                dmmdtnascimento_m.focus();
                erro = 1;
                return false;
            }
            if( !estuf_m.val() ){
                alert('O campo "Getor Local - UF" é um campo obrigatório!');
                estuf_m.focus();
                erro = 1;
                return false;
            }
            if( !muncodend_m.val() ){
                alert('O campo "Getor Local - Município" é um campo obrigatório!');
                muncod_m.focus();
                erro = 1;
                return false;
            }
            if( !dmmorgao_m.val() ){
                alert('O campo "Getor Local - Órgão" é um campo obrigatório!');
                dmmorgao_m.focus();
                erro = 1;
                return false;
            }
            if( !dmmfonecomercial_m.val() ){
                alert('O campo "Getor Local - Telefone Fixo Comercial" é um campo obrigatório!');
                dmmfonecomercial_m.focus();
                erro = 1;
                return false;
            }
            if( !dmmcelular_m.val() ){
                alert('O campo "Getor Local - Telefone Celular" é um campo obrigatório!');
                dmmcelular_m.focus();
                erro = 1;
                return false;
            }
            if( !dmmemail_m.val() ){
                alert('O campo "Getor Local - E-mail" é um campo obrigatório!');
                dmmemail_m.focus();
                erro = 1;
                return false;
            }
            if( !dmmcargofuncao_m.val() ){
                alert('O campo "Getor Local - Cargo/Função" é um campo obrigatório!');
                dmmcargofuncao_m.focus();
                erro = 1;
                return false;
            }

            if(!erro){
                $('#requisicao').val('salvarDadosMaisMedicos');
                $('#formulario').submit();
            }
        });
    });

    function atualizaComboMunicipio( estuf, tipo ){
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=atualizaComboMunicipio&tipo="+tipo+"&estuf="+estuf,
            //async: false,
            asynchronous: false,
            success: function(resp){
                if(tipo == 'S'){
                    $('#td_combo_mum_s').html(resp);
                }else{
                    $('#td_combo_mum_m').html(resp);
                }
            }
        });
    }

    function verificarCPFValido( dmmcpf, tipo ){
        if(dmmcpf != ''){
            var valido = validar_cpf( dmmcpf );

            if(!valido){
                alert( "CPF inválido! Favor informar um CPF válido!" );
                if(tipo == 'S'){
                	$('#dmmcpf_s').focus();
                }else{
	                $('#dmmcpf_m').focus();
                }
                return false;
            }
        }
    }

</script>

<form name="formulario" id="formulario" method="post" action="">
    <input type="hidden" id="requisicao" name="requisicao" value=""/>
    <input type="hidden" id="dmmid_s" name="dmmid_s" value="<?php echo $dados_getor_s['dmmid']; ?>"/>
    <input type="hidden" id="dmmid_m" name="dmmid_m" value="<?php echo $dados_getor_m['dmmid']; ?>"/>
    <input type="hidden" id="prgid" name="prgid" value="<?php echo $dados_getor_s['prgid']; ?>"/>
    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td>
                <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                    <tr>
                        <td bgcolor="#CCCCCC" colspan="2">&nbsp;<b>Dados do Dirigente Municipal</b></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" width="25%">Nome Completo:</td>
                        <td><?php echo $prefeito['entnome']; ?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">CPF:</td>
                        <td><?php echo formatar_cpf($prefeito['entnumcpfcnpj']); ?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">RG:</td>
                        <td><?php echo $prefeito['entnumrg'] .' - '. $prefeito['entorgaoexpedidor']; ?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Sexo:</td>
                        <td><?php echo $prefeito['entsexo'] == 'M' ? 'Masculino' : 'Feminino'; ?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Data de nascimento:</td>
                        <td><?php echo formata_data($prefeito['entdatanasc']); ?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">UF:</td>
                        <td><?php echo $prefeito['estuf']; ?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Município:</td>
                        <td><?php echo $prefeito['mundescricao']; ?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Órgão:</td>
                        <td><?php echo $prefeito['orgao']; ?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Telefone Fixo Comercial:</td>
                        <td><?php echo '(' . trim( $prefeito['entnumdddcomercial'] ) . ') - ' . $prefeito['entnumcomercial']; ?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Telefone Celular:</td>
                        <td><?php echo '(' . trim( $prefeito['entnumdddcelular'] ) . ') - ' . $prefeito['entnumcelular']; ?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">e-mail:</td>
                        <td><?php echo $prefeito['entemail']; ?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Cargo/Função:</td>
                        <td><?php echo $prefeito['cargo_func']; ?></td>
                    </tr>
                </table>
                <br>
                <!-- DADOS DO GESTOR DO SUS -->
                <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                    <tr>
                        <td bgcolor="#CCCCCC" colspan="2">&nbsp;<b>Dados do Gestor Local do Sistema Único de Saúde (SUS)</b></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" width="25%">Nome Completo:</td>
                        <td><?php echo campo_texto('dmmnome_s', 'S', $habilitado, 'Nome Completo', '50', '150', '', '', '', '', '', 'id="dmmnome_s"', '', $dados_getor_s['dmmnome']); ?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">CPF:</td>
                        <td><?php echo campo_texto('dmmcpf_s', 'S', $habilitado, 'CPF', '50', '14', '###.###.###-##', '', '', '', '', 'id="dmmcpf_s"', '', $dados_getor_s['dmmcpf'], 'verificarCPFValido(this.value, \'S\')'); ?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">RG:</td>
                        <td><?php echo campo_texto('dmmrg_s', 'S', $habilitado, 'RG', '50', '14', '', '', '', '', '', 'id="dmmrg_s"', '', $dados_getor_s['dmmrg']); ?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Sexo:</td>
                        <td>
                            <input type="radio" name="dmmsexo_s" id="dmmsexo_s[m]" <? if ($dados_getor_s['dmmsexo'] == 'm') echo 'checked = checked'; ?> value="m"> Masculino &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                            <input type="radio" name="dmmsexo_s" id="dmmsexo_s[f]" <? if ($dados_getor_s['dmmsexo'] == 'f') echo 'checked = checked'; ?> value="f"> Feminino
                            <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Data de Nascimento:</td>
                        <td><?php echo campo_data2('dmmdtnascimento_s', 'S', $habilitado, 'Data de Nascimento', 'S', '', '', $dados_getor_s['dmmdtnascimento']); ?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">UF:</td>
                        <td>
                            <?php
                                $estuf = $dados_getor_s['estuf'] ? $dados_getor_s['estuf'] : $prefeito['estuf'];
                                $sql = "SELECT  estuf AS codigo,
                                                    estuf AS descricao
                                            FROM 	territorios.estado ORDER BY estuf";
                                $db->monta_combo('estuf_s',$sql,$habilitado,'Selecione...','atualizaComboMunicipio(this.value, \'S\');','','','372','S','estuf_s','', $estuf); ?>
                        </td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">Município:</td>
                            <td id="td_combo_mum_s">
                                <?php
                                    $muncodend = $dados_getor_s['muncodend'] ? $dados_getor_s['muncodend'] : $prefeito['muncod'];
                                    $sql = "SELECT  	muncod AS codigo,
                                                            mundescricao AS descricao
                                            FROM 		territorios.municipio
                                            ORDER BY	mundescricao";
                                    $db->monta_combo('muncodend_s',$sql,$habilitado,'Selecione...','','','','372','S','muncodend_s','', $muncodend); ?>
                            </td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">Órgão:</td>
                            <td><?php echo campo_texto('dmmorgao_s', 'S', $habilitado, 'Órgão', '50', '150', '', '', '', '', '', 'id="dmmorgao_s"', '', $dados_getor_s['dmmorgao']); ?></td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">Telefone Fixo Comercial:</td>
                            <td><?php echo campo_texto('dmmfonecomercial_s', 'S', $habilitado, 'Telefone Fixo Comercial', '50', '150', '##-####-####', '', '', '', '', 'id="dmmfonecomercial_s"', '', $dados_getor_s['dmmfonecomercial']); ?></td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">Telefone Celular:</td>
                            <td><?php echo campo_texto('dmmcelular_s', 'S', $habilitado, 'Telefone Celular', '50', '150', '##-####-####', '', '', '', '', 'id="dmmcelular_s"', '', $dados_getor_s['dmmcelular']); ?></td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">e-mail:</td>
                            <td><?php echo campo_texto('dmmemail_s', 'S',$habilitado, 'e-mail', '50', '150', '', '', '', '', '', 'id="dmmemail_s"', '', $dados_getor_s['dmmemail']); ?></td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">Cargo/Função:</td>
                            <td><?php echo campo_texto('dmmcargofuncao_s', 'S',$habilitado, 'Cargo/Função', '50', '150', '', '', '', '', '', 'id="dmmcargofuncao_s"', '', $dados_getor_s['dmmcargofuncao']); ?></td>
                        </tr>

                        <!-- DADOS DO REPRESENTANTE LEGAL DO MUNICIPIO -->
                        <tr>
                            <td bgcolor="#CCCCCC" colspan="2">&nbsp;<b>Dados do Representante Legal do Município no Acompanhamento desde Edital</b></td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">Nome Completo:</td>
                            <td><?php echo campo_texto('dmmnome_m', 'S', $habilitado, 'Nome Completo', '50', '150', '', '', '', '', '', 'id="dmmnome_m"', '', $dados_getor_m['dmmnome']); ?></td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">CPF:</td>
                            <td><?php echo campo_texto('dmmcpf_m', 'S', $habilitado, 'CPF', '50', '14', '###.###.###-##', '', '', '', '', 'id="dmmcpf_m"', '', $dados_getor_m['dmmcpf'], 'verificarCPFValido(this.value, \'M\')'); ?></td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">RG:</td>
                            <td><?php echo campo_texto('dmmrg_m', 'S', $habilitado, 'RG', '50', '14', '', '', '', '', '', 'id="dmmrg_m"', '', $dados_getor_m['dmmrg']); ?></td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">Sexo:</td>
                            <td>
                                <input type="radio" name="dmmsexo_m" id="dmmsexo_m[m]" <? if ($dados_getor_m['dmmsexo'] == 'm') echo 'checked = checked'; ?> value="m"> Masculino &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                                <input type="radio" name="dmmsexo_m" id="dmmsexo_m[f]" <? if ($dados_getor_m['dmmsexo'] == 'f') echo 'checked = checked'; ?> value="f"> Feminino
                                <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
                            </td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">Data de Nascimento:</td>
                            <td><?php echo campo_data2('dmmdtnascimento_m', 'S', $habilitado, 'Data de Nascimento', 'S', '', '', $dados_getor_m['dmmdtnascimento']); ?></td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">UF:</td>
                            <td>
                                <?php
                                    $estuf_m = $dados_getor_m['estuf'] ? $dados_getor_m['estuf'] : $prefeito['estuf'];
                                    $sql = "SELECT 		estuf AS codigo,
                                                        estuf AS descricao
                                            FROM 		territorios.estado ORDER BY estuf";
                                    $db->monta_combo('estuf_m',$sql,$habilitado,'Selecione...','atualizaComboMunicipio(this.value, \'M\');','','','372','S','estuf_m','', $estuf_m); ?>
                            </td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">Município:</td>
                            <td id="td_combo_mum_m">
                                <?php
                                    $muncodend = $dados_getor_m['muncodend'] ? $dados_getor_m['muncodend'] : $prefeito['muncod'];
                                    $sql = "SELECT  	muncod AS codigo,
                                                            mundescricao AS descricao
                                            FROM 		territorios.municipio
                                            ORDER BY 	mundescricao";
                                    $db->monta_combo('muncodend_m',$sql,$habilitado,'Selecione...','','','','372','S','muncodend_m','', $muncodend);
                                ?>
                            </td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">Órgão:</td>
                            <td><?php echo campo_texto('dmmorgao_m', 'S', $habilitado, 'Órgão', '50', '150', '', '', '', '', '', 'id="dmmorgao_m"', '', $dados_getor_m['dmmorgao']); ?></td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">Telefone Fixo Comercial:</td>
                            <td><?php echo campo_texto('dmmfonecomercial_m', 'S', $habilitado, 'Telefone Fixo Comercial', '50', '150', '##-####-####', '', '', '', '', 'id="dmmfonecomercial_m"', '', $dados_getor_m['dmmfonecomercial']); ?></td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">Telefone Celular:</td>
                            <td><?php echo campo_texto('dmmcelular_m', 'S', $habilitado, 'Telefone Celular', '50', '150', '##-####-####', '', '', '', '', 'id="dmmcelular_m"', '', $dados_getor_m['dmmcelular']); ?></td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">e-mail:</td>
                            <td><?php echo campo_texto('dmmemail_m', 'S', $habilitado, 'e-mail', '50', '150', '', '', '', '', '', 'id="dmmemail_m"', '', $dados_getor_m['dmmemail']); ?></td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">Cargo/Função:</td>
                            <td><?php echo campo_texto('dmmcargofuncao_m', 'S', $habilitado, 'Cargo/Função', '50', '150', '', '', '', '', '', 'id="dmmcargofuncao_m"', '', $dados_getor_m['dmmcargofuncao']); ?></td>
                        </tr>
                </table>
            </td>
                <?PHP
                    if($docid){
                        echo '<td align="right" valign="top" width="5%">';
                        wf_desenhaBarraNavegacao($docid, array('rqmid' => $_SESSION['cap']['rqmid']), '');
                        echo '</td>';
                    }
                ?>
        </tr>
    </table>
    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td colspan="2" class="SubTituloCentro">
                <?PHP
                    if( $habilitado == 'S' ){
                ?>
                    <input id="salvarDadosMaisMedicos" type="button" value="Salvar" name="salvarDadosMaisMedicos"/>
                <?PHP
                    }else{
                ?>
                    <input id="salvar_disabilitado" type="button" value="Salvar" name="salvar_disabilitado" disabled="disabled"/>
                <?PHP
                    }
                ?>
                <input id="btnVoltar" type="button" value="Voltar" name="btnVoltar" onclick="window.location.href='?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';" />
            </td>
        </tr>
    </table>
</form>
