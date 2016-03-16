<?php 
    if (!$_SESSION['projovemurbano']['pjuid']) {
        die("
            <script>
                alert('Problema encontrado no carregamento. Inicie novamente a navegação.');
                window.location='projovemurbano.php?modulo=inicio&acao=C';
           </script>
        ");
    }

    if ($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
        exit;
    }
    
    $bloqueioHorario = bloqueioadesao2014();
    
    $habilita = 'N';
    $perfil = pegaPerfilGeral();
    if( ( in_array(PFL_SECRETARIO_MUNICIPAL, $perfil) || in_array(PFL_SECRETARIO_ESTADUAL, $perfil) )  && 
    	$_SESSION['projovemurbano']['ppuano'] >= date('Y') && !$bloqueioHorario) {
        $habilita = 'S';
    }
    if( in_array(PFL_EQUIPE_MEC, $perfil) || in_array(PFL_ADMINISTRADOR, $perfil) || $db->testa_superuser()  ){
    	$habilita = 'S';
    }

    include_once APPRAIZ . 'includes/cabecalho.inc';
    require_once APPRAIZ . "www/includes/webservice/cpf.php";
    echo '<br>';


    echo montarAbasArray(montaMenuProJovemUrbano(), $_SERVER['REQUEST_URI']);

    monta_titulo('Projovem Urbano', montaTituloEstMun());

    $usuarioprojovem = pegarUsuarioProJovem();

    $rsSecretaria = recuperaSecretariaPorUfMuncod();
    
?>
    <script type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script>

        function salvarIdentificacao() {
            if (jQuery('#iseregistrogeral').val() == "") {
                alert('Registro Geral é obrigatório');
                return false;
            }
            if (jQuery('#iseorgaoexpedidor').val() == "") {
                alert('Órgão Emissor/Expedidor é obrigatório');
                return false;
            }
            if (jQuery('#isecelularddd').val() == "") {
                alert('DDD Celular é obrigatório');
                return false;
            }
            if (jQuery('#isecelular').val() == "") {
                alert('Celular é obrigatório');
                return false;
            }
            if (jQuery('#isetelefoneddd').val() == "") {
                alert('DDD Telefone é obrigatório');
                return false;
            }
            if (jQuery('#isetelefone').val() == "") {
                alert('Telefone é obrigatório');
                return false;
            }
            if (jQuery('#isecep').val() == "") {
                alert('CEP é obrigatório');
                return false;
            }
            if (jQuery('#iseendereco').val() == "") {
                alert('Endereço é obrigatório');
                return false;
            }
            if (jQuery('#isebairro').val() == "") {
                alert('Bairro é obrigatório');
                return false;
            }
            if (jQuery('#isenumero').val() == "") {
                alert('Número é obrigatório');
                return false;
            }
            if (jQuery('#iseuf').val() == "") {
                alert('UF é obrigatório');
                return false;
            }
            if (jQuery('#isemunicipio').val() == "") {
                alert('Município é obrigatório');
                return false;
            }

            jQuery('#form').submit();
        }

        function getEnderecoPeloCEP(cep) {
            jQuery.ajax({
                type: "POST",
                url: "/geral/consultadadosentidade.php",
                data: "requisicao=pegarenderecoPorCEP&endcep=" + cep,
                async: false,
                success: function(dados) {
                    var enderecos = dados.split("||");
                    jQuery('#iseendereco').val(enderecos[0]);
                    jQuery('#isebairro').val(enderecos[1]);
                    jQuery('#mundescricao').val(enderecos[2]);
                    jQuery('#iseuf').val(enderecos[3]);
                    jQuery('#isemunicipio').val(enderecos[4]);
                }
            });
        }
   
        function alterarCPF( cpf ){
        
            var cpf = jQuery('#novo_cpf').val();
            
            //VALIDAR CPF.
            if( !validar_cpf( cpf ) ){
                alert('CPF inválido!');
                $('#novo_cpf').focus();
                $('#salvar').attr('disabled', 'true');
                return false;		
            }else if( validar_cpf( cpf ) ){
                $('#salvar').removeAttr('disabled');
            }
            
            //VALIDAR CPF RECEITA
            var comp = new dCPF();
            comp.buscarDados(cpf);
            //console.log(comp.dados);
            if( comp.dados.no_pessoa_rf != '' ){
                jQuery('#td_nome_secretario').html(comp.dados.no_pessoa_rf);
                jQuery('[name=isecpf]').val(comp.dados.nu_cpf_rf);
            }else{
                alert('CPF não encontrado na base de dados da receita.' );
                return false;
            }
            //VERIFICA E AUALIZA NA BASE DE DADOS DO SISTEMA.
            jQuery.ajax({
                type: "POST",
                url: window.location,
                data: "requisicao=alterarCpfNovo&novo_cpf=" + cpf,
                async: false,
                success: function(result) {
                    alert(result);
                }
            });
        }
    </script>
    
    <form id="form" name="form" method="POST">
        <input type="hidden" name="requisicao" value="<?= (($usuarioprojovem['isecid']) ? "atualizarIdentificacao" : "inserirIdentificacao") ?>">
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
            <tr>
                <td colspan="2" class="SubTituloDireita">CNPJ da Secretaria:</td>
                <td>
                    <?php
                        $cnpjsecretaria = formatar_cnpj($rsSecretaria['entnumcpfcnpj']);
                        echo campo_texto('cnpjsecretaria', 'N', 'N', '', 45, '', '##.###.###/####-##', '');
                    ?>
                </td>
            </tr>
            <? //if (in_array(PFL_EQUIPE_MEC, $perfil) || $db->testa_superuser()) : ?>
            <td rowspan="6" class="SubTituloDireita">Secretário(a) de Educação:</td>
            <tr>
                <td class="SubTituloDireita">
                    <?php echo $usuarioprojovem['usunome'] ? '<b>Novo CPF:</b>' : 'CPF:'; ?>
                </td>

                <td>
                    <?php
                        if($usuarioprojovem['usucpf']){
                            $cpf = formatar_cpf( $usuarioprojovem['usucpf'] );
                        }else{
                            $cpf = formatar_cpf( $_SESSION['usucpf'] );
                        }
                        echo campo_texto('novo_cpf', 'N', $habilita, 'Novo CPF', 15, 14, "###.###.###-##", '', '', '', 0, 'id="novo_cpf"', '', $cpf, 'alterarCPF();');
                    ?>
                        <font size=1>Para alterar o usuário, digite um novo CPF e clique em alterar CPF</font> 
                        <input type="button" name="alterarnovocpf" value="Alterar CPF" onclick="alterarCPF();">
                </td>
            </tr>
            <? //endif; ?>
            <tr>
                <td class="SubTituloDireita">Nome Completo:</td>
                <td>
                    <?php
                        $usunome = $db->pegaUm("select usunome from seguranca.usuario where usucpf = '{$_SESSION['usucpf']}'");
                    ?>
                    <input type="hidden" name="isecpf" value="<? echo (($usuarioprojovem['isecpf']) ? $usuarioprojovem['isecpf'] : $_SESSION['usucpf']); ?>">
                    <span id="td_nome_secretario"><? echo (($usuarioprojovem['usunome']) ? $usuarioprojovem['usunome'] : $usunome); ?></span>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Registro Geral (RG):</td>
                <td><? echo campo_texto('iseregistrogeral', 'S', $habilita, 'Registro Geral(RG)', 20, 17, "", "", '', '', 0, 'id="iseregistrogeral"', '', $usuarioprojovem['iserg']); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Órgão Emissor/Expedidor:</td>
                <td><? echo campo_texto('iseorgaoexpedidor', "S", $habilita, "Nome do grupo", 20, 30, "", "", '', '', 0, 'id="iseorgaoexpedidor"', '', $usuarioprojovem['iseorgexp']); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Telefone Celular:</td>
                <td>( <? echo campo_texto('isecelularddd', "N", $habilita, "DDD Telefone Celular", 4, 2, "###", "", '', '', 0, 'id="isecelularddd"', '', substr($usuarioprojovem['isecelular'], 0, 2)); ?> ) <? echo campo_texto('isecelular', "S", $habilita, "Telefone Celular", 17, 15, "#####-####", "", '', '', 0, 'id="isecelular"', '', mascaraglobal(substr($usuarioprojovem['isecelular'], 2), "####-####")); ?></td>
            </tr>
            
            <!-- DADOS DA SECRETARIA -->
            <td rowspan="9" class="SubTituloDireita">Secretaria de Educação:</td>
            
            <tr>
                <td class="SubTituloDireita">Telefone:</td>
                <td>( <? echo campo_texto('isetelefoneddd', "N", $habilita, "DDD Telefone", 4, 2, "###", "", '', '', 0, 'id="isetelefoneddd"', '', substr($usuarioprojovem['isetelefone'], 0, 2)); ?> ) <? echo campo_texto('isetelefone', "S", $habilita, "Telefone", 17, 15, "#####-####", "", '', '', 0, 'id="isetelefone"', '', mascaraglobal(substr($usuarioprojovem['isetelefone'], 2), "####-####")); ?></td>
            </tr>
            
            <tr>
                <td class="SubTituloDireita">CEP:</td>
                <td><? echo campo_texto('isecep', "S", $habilita, "CEP", 10, 9, "#####-###", "", '', '', 0, 'id="isecep"', '', mascaraglobal($usuarioprojovem['isecep'], "#####-###"), 'getEnderecoPeloCEP(this.value);'); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Endereço</td>
                <td><? echo campo_texto('iseendereco', "S", $habilita, "CEP", 50, 200, "", "", '', '', 0, 'id="iseendereco"', '', $usuarioprojovem['iseendereco']); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Bairro:</td>
                <td><? echo campo_texto('isebairro', "S", $habilita, "CEP", 50, 200, "", "", '', '', 0, 'id="isebairro"', '', $usuarioprojovem['isebairro']); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Complemento:</td>
                <td><? echo campo_texto('isecomplemento', "N", $habilita, "CEP", 50, 200, "", "", '', '', 0, 'id="isecomplemento"', '', $usuarioprojovem['isecomplemento']); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Número:</td>
                <td><? echo campo_texto('isenumero', "S", $habilita, "CEP", 9, 10, "#########", "", '', '', 0, 'id="isenumero"', '', $usuarioprojovem['isenumero']); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita">UF:</td>
                <td><? echo campo_texto('iseuf', "N", "N", "UF", 2, 4, "", "", '', '', 0, 'id="iseuf"', '', $usuarioprojovem['iseuf']); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Municipio:</td>
                <td>
                    <input type="hidden" name="isemunicipio" id="isemunicipio" value="<?= $usuarioprojovem['isemunicipio'] ?>">
    <? echo campo_texto('mundescricao', "N", "N", "Município", 50, 200, "", "", '', '', 0, 'id="mundescricao"', '', (($usuarioprojovem['isemunicipio']) ? $db->pegaUm("SELECT mundescricao FROM territorios.municipio WHERE muncod='" . $usuarioprojovem['isemunicipio'] . "'") : "")); ?>
                </td>
            </tr>
    <?php if ($habilita == 'S') { ?>
                <tr>
                    <td class="SubTituloCentro" colspan="3">
                        <input type="button" id="salvar" name="salvar" value="Salvar" onclick="salvarIdentificacao();">
                    </td>
                </tr>
    <?php } ?>
        </table>
    </form>
    
    <? registarUltimoAcesso(); ?>
