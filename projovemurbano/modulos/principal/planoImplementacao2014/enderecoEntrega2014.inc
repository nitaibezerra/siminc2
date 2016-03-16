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
//     $dataAtual      = mktime( date('H'), date('i'), date('s'), date('m'), date('d'), date('Y') );
//     $dataBloqueio   = mktime( 23, 59, 00, 08,6, 2014 );
//     $bloqueioHorario = (bool) ( $dataAtual > $dataBloqueio );
    $habilita = 'N';
    $perfil = pegaPerfilGeral();
    if( ( in_array(PFL_SECRETARIO_MUNICIPAL, $perfil) || in_array(PFL_SECRETARIO_ESTADUAL, $perfil)|| in_array(PFL_COORDENADOR_ESTADUAL, $perfil) || in_array(PFL_COORDENADOR_MUNICIPAL, $perfil)) /* && 
    	$_SESSION['projovemurbano']['ppuano'] >= date('Y') && !$bloqueioHorario*/) {
        $habilita = 'S';
    }
    if( ( in_array(PFL_EQUIPE_MEC, $perfil) || in_array(PFL_ADMINISTRADOR, $perfil) || $db->testa_superuser()  && !$bloqueioHorario) ){
    	$habilita = 'S';
    }

    include_once APPRAIZ . 'includes/cabecalho.inc';
    require_once APPRAIZ . "www/includes/webservice/cpf.php";
    echo '<br>';


    $usuarioprojovem = pegarUsuarioMaterial();

    $rsSecretaria = recuperaSecretariaPorUfMuncod();
    
?>
    <script type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script>

        function salvarenderecoresp() {
            if (jQuery('#novo_cpf').val() == "") {
                alert('CPF é obrigatório');
                return false;
            }
            if (jQuery('#iseorgaoexpedidor').val() == "") {
                alert('Órgão Emissor/Expedidor é obrigatório');
                return false;
            }
            if (jQuery('#eemcep').val() == "") {
                alert('CEP é obrigatório');
                return false;
            }
            if (jQuery('#eemlogradouro').val() == "") {
                alert('Endereço é obrigatório');
                return false;
            }
            if (jQuery('#eembairro').val() == "") {
                alert('Bairro é obrigatório');
                return false;
            }
            if (jQuery('#eemnumero').val() == "") {
                alert('Número é obrigatório');
                return false;
            }
            if (jQuery('#eemuf').val() == "") {
                alert('UF é obrigatório');
                return false;
            }
            if (jQuery('#eemmuncod').val() == "") {
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
                    jQuery('#eemlogradouro').val(enderecos[0]);
                    jQuery('#eembairro').val(enderecos[1]);
                    jQuery('#mundescricao').val(enderecos[2]);
                    jQuery('#eemuf').val(enderecos[3]);
                    jQuery('#eemmuncod').val(enderecos[4]);
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
                jQuery('[name=eemcpfresponsavel]').val(comp.dados.nu_cpf_rf);
            }else{
                alert('CPF não encontrado na base de dados da receita.' );
                return false;
            }
        }
    </script>
    <form id="form" name="form" method="POST">
        <input type="hidden" name="requisicao" value="<?= (($usuarioprojovem['eemid']) ? "atualizarResponsavelMaterial" : "inserirResponsavelMaterial") ?>">
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
            <? //if (in_array(PFL_EQUIPE_MEC, $perfil) || $db->testa_superuser()) : ?>
            <td rowspan="3" class="SubTituloDireita">Responsável pelo recebimento do material:</td>
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
                        echo campo_texto('novo_cpf', 'S', 'S', 'Novo CPF', 15, 14, "###.###.###-##", '', '', '', 0, 'id="novo_cpf"', '', $cpf, 'alterarCPF(\'novo_cpf\');');
                    ?>
                </td>
            </tr>
            <? //endif; ?>
            <tr>
                <td class="SubTituloDireita">Nome Completo:</td>
                <td>
                    <?php
                        $usunome = $db->pegaUm("select usunome from seguranca.usuario where usucpf = '{$_SESSION['usucpf']}'");
                    ?>
                    <input type="hidden" name="eemcpfresponsavel" value="<? echo (($usuarioprojovem['eemcpfresponsavel']) ? $usuarioprojovem['eemcpfresponsavel'] : $_SESSION['usucpf']); ?>">
                    <input type="hidden" name="eemnomeresponsavel" value="<? echo (($usuarioprojovem['usunome']) ? $usuarioprojovem['usunome'] : $usunome); ?>">
                    <input type="hidden" name="eemid" value="<? echo ($usuarioprojovem['eemid']); ?>">
                    <span id="td_nome_secretario"><? echo (($usuarioprojovem['usunome']) ? $usuarioprojovem['usunome'] : $usunome); ?></span>
                </td>
            </tr>
            <td rowspan="8" class="SubTituloDireita">Endereço de recebimento do material:</td>
            <tr>
                <td class="SubTituloDireita">CEP:</td>
                <td><? echo campo_texto('eemcep', "S", $habilita, "CEP", 10, 9, "#####-###", "", '', '', 0, 'id="eemcep"', '', mascaraglobal($usuarioprojovem['eemcep'], "#####-###"), 'getEnderecoPeloCEP(this.value);'); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Endereço</td>
                <td><? echo campo_texto('eemlogradouro', "S", 'N', "CEP", 50, 200, "", "", '', '', 0, 'id="eemlogradouro"', '', $usuarioprojovem['eemlogradouro']); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Bairro:</td>
                <td><? echo campo_texto('eembairro', "S", 'N', "CEP", 50, 200, "", "", '', '', 0, 'id="eembairro"', '', $usuarioprojovem['eembairro']); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Complemento:</td>
                <td><? echo campo_texto('eemcomplemento', "N", $habilita, "CEP", 50, 200, "", "", '', '', 0, 'id="eemcomplemento"', '', $usuarioprojovem['eemcomplemento']); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Número:</td>
                <td><? echo campo_texto('eemnumero', "S", $habilita, "CEP", 9, 10, "#########", "", '', '', 0, 'id="eemnumero"', '', $usuarioprojovem['eemnumero']); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita">UF:</td>
                <td><? echo campo_texto('eemuf', "N", 'N', "UF", 2, 4, "", "", '', '', 0, 'id="eemuf"', '', $usuarioprojovem['eemuf']); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Municipio:</td>
                <td>
                    <input type="hidden" name="eemmuncod" id="eemmuncod" value="<?= $usuarioprojovem['eemmuncod'] ?>">
    <? echo campo_texto('mundescricao', "N", "N", "Município", 50, 200, "", "", '', '', 0, 'id="mundescricao"', '', (($usuarioprojovem['eemmuncod']) ? $db->pegaUm("SELECT mundescricao FROM territorios.municipio WHERE muncod='" . $usuarioprojovem['eemmuncod'] . "'") : "")); ?>
                </td>
            </tr>
    <?php if ($habilita == 'S') { ?>
                <tr>
                    <td class="SubTituloCentro" colspan="3">
                        <input type="button" id="salvar" name="salvar" value="Salvar" onclick="salvarenderecoresp();">
                    </td>
                </tr>
    <?php } ?>
        </table>
    </form>
    
    <? registarUltimoAcesso(); ?>
