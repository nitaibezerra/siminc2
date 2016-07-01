<?php 
   
    if ($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
        exit;
    }
    $habilita = 'N';
    
    $perfil = pegaPerfilGeral();
    
    if(!in_array(PFL_EQUIPE_MEC, $perfil) && !in_array(PFL_ADMINISTRADOR, $perfil) && !$db->testa_superuser()){
    	$bloqueioHorario = bloqueioadesao();
    }
    
    if( (in_array(PFL_EQUIPE_MEC, $perfil) || in_array(PFL_ADMINISTRADOR, $perfil) || $db->testa_superuser()
    		||in_array(PFL_SECRETARIO_MUNICIPAL, $perfil) || in_array(PFL_SECRETARIO_ESTADUAL, $perfil) )&&!$bloqueioHorario ) {
        $habilita = 'S';
    }
//     if( ( in_array(PFL_EQUIPE_MEC, $perfil) || in_array(PFL_ADMINISTRADOR, $perfil) || $db->testa_superuser() ) ){
//     	$habilita = 'S';
//     }
//     $habilita = 'S';
    
    include_once APPRAIZ . 'includes/cabecalho.inc';
    require_once APPRAIZ . "www/includes/webservice/cpf.php";
    echo '<br>';


    echo montarAbasArray(montaMenuProjovemCampo(), $_SERVER['REQUEST_URI']);

    monta_titulo('Projovem Campo', montaTituloEstMun());

    $usuarioprojovem = pegarUsuarioProJovemCampo();

    $rsSecretaria = recuperaSecretariaPorUfMuncod();

?>
    <script type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script>

        function salvarIdentificacao() {
            if (jQuery('#seconumrg').val() == "") {
                alert('Registro Geral é obrigatório');
                return false;
            }
            if (jQuery('#secoorgaoexprg').val() == "") {
                alert('Órgão Emissor/Expedidor é obrigatório');
                return false;
            }
            if (jQuery('#secocelularddd').val() == "") {
                alert('DDD Celular é obrigatório');
                return false;
            }
            if (jQuery('#secocelular').val() == "") {
                alert('Celular é obrigatório');
                return false;
            }
            if (jQuery('#secatelefoneddd').val() == "") {
                alert('DDD Telefone é obrigatório');
                return false;
            }
            if (jQuery('#secatelefone').val() == "") {
                alert('Telefone é obrigatório');
                return false;
            }
            if (jQuery('#secacep').val() == "") {
                alert('CEP é obrigatório');
                return false;
            }
            if (jQuery('#secaendereco').val() == "") {
                alert('Endereço é obrigatório');
                return false;
            }
            if (jQuery('#secabairro').val() == "") {
                alert('Bairro é obrigatório');
                return false;
            }
            if (jQuery('#secanumero').val() == "") {
                alert('Número é obrigatório');
                return false;
            }
            if (jQuery('#secauf').val() == "") {
                alert('UF é obrigatório');
                return false;
            }
            if (jQuery('#secamuncod').val() == "") {
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
                    jQuery('#secaendereco').val(enderecos[0]);
                    jQuery('#secabairro').val(enderecos[1]);
                    jQuery('#mundescricao').val(enderecos[2]);
                    jQuery('#secauf').val(enderecos[3]);
                    jQuery('#secamuncod').val(enderecos[4]);
                }
            });
        }
   
        function alterarCPF( cpf ){

            var cpf = jQuery('#secocpf').val();
            
            //VALIDAR CPF.
            if( !validar_cpf( cpf ) ){
                alert('CPF inválido!');
                $('#secocpf').focus();
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
                jQuery('#seconome').val(comp.dados.no_pessoa_rf);
                jQuery('[name=secocpf]').val(comp.dados.nu_cpf_rf);
            }else{
                alert('CPF não encontrado na base de dados da receita.' );
                return false;
            }
            //VERIFICA E AUALIZA NA BASE DE DADOS DO SISTEMA.
            jQuery.ajax({
                type: "POST",
                url: window.location,
                data: "requisicao=alterarCpfNovo&secocpf=" + cpf,
                async: false,
                success: function(result) {
//                     alert(result);
                }
            });
        }

    </script>
    
    <form id="form" name="form" method="POST">
        <input type ="hidden" name="requisicao" value="inserirIdentificacao">
        <input type ="hidden" name="secaid" id="secaid" value="<?= $usuarioprojovem['secaid'] ?>" />
        <input type ="hidden" name="secoid" id="secoid" value="<?= $usuarioprojovem['secoid'] ?>" />
        
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
            <tr>
                <td colspan="2" class="SubTituloDireita">CNPJ da Secretaria:</td>
                <td>
                    <?php
                    	
                        
                        if( !$rsSecretaria['entnumcpfcnpj'] || $rsSecretaria['entnumcpfcnpj'] == '' ){
                        	$secacnpj = formatar_cnpj(recuperaCNPJPrefeitura() );
                        }else{
							$secacnpj = formatar_cnpj($rsSecretaria['entnumcpfcnpj']);
						}
                        echo campo_texto('secacnpj', 'N', 'N', '', 45, '', '##.###.###/####-##', '');
                    ?>
                </td>
            </tr>
            <? //if (in_array(PFL_EQUIPE_MEC, $perfil) || $db->testa_superuser()) : ?>
            <td rowspan="6" class="SubTituloDireita">Secretário(a) de Educação:</td>
            <tr>
                <td class="SubTituloDireita">
                    <?php echo $usuarioprojovem['seconome'] ? '<b>Novo CPF:</b>' : 'CPF:'; ?>
                </td>

                <td>
                    <?php
                        if($usuarioprojovem['secocpf']){
                            $cpf = formatar_cpf( $usuarioprojovem['secocpf'] );
                        }else{
                            $cpf = formatar_cpf( $_SESSION['usucpf'] );
                        }
                        echo campo_texto('secocpf', 'N', $habilita, 'CPF', 15, 14, "###.###.###-##", '', '', '', 0, 'id="secocpf"', '', $cpf, 'alterarCPF();');
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
                    	if($usuarioprojovem['secocpf']){
                        	$secnome = $usuarioprojovem['seconome'];
                        }else{
							$seconome = $db->pegaUm("select usunome from seguranca.usuario where usucpf = '{$_SESSION['usucpf']}'");
                    	}
					?>
                    <input type="hidden" name="secocpf" value="<? echo (($usuarioprojovem['secocpf']) ? $usuarioprojovem['secocpf'] : $_SESSION['usucpf']); ?>">
                     <input type="hidden" id="seconome" name="seconome" value="<? echo (($usuarioprojovem['seconome']) ? $usuarioprojovem['seconome'] : $seconome); ?>">
                    <span id="td_nome_secretario"><? echo (($usuarioprojovem['seconome']) ? $usuarioprojovem['seconome'] : $seconome); ?></span>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Registro Geral (RG):</td>
                <td><? echo campo_texto('seconumrg', 'S', $habilita, 'Registro Geral(RG)', 20, 10, "", "", '', '', 0, 'id="seconumrg"', '', $usuarioprojovem['seconumrg']); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Órgão Emissor/Expedidor:</td>
                <td><? echo campo_texto('secoorgaoexprg', "S", $habilita, "Nome do grupo", 20, 30, "", "", '', '', 0, 'id="secoorgaoexprg"', '', $usuarioprojovem['secoorgaoexprg']); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Telefone Celular:
                <?php 
					$cel = substr($usuarioprojovem['secocelular'], 2);
					$tel = substr($usuarioprojovem['secatelefone'], 2);
				?>
                </td>
                <td>( <? echo campo_texto('secocelularddd', "N", $habilita, "DDD Telefone Celular", 4, 2, "###", "", '', '', 0, 'id="secocelularddd"', '', substr($usuarioprojovem['secocelular'], 0, 2)); ?> ) <? echo campo_texto('secocelular', "S", $habilita, "Telefone Celular", 17, 15, "#####-####", "", '', '', 0, 'id="secocelular"', '', mascaraglobal( $cel, "####-#####")); ?></td>
            </tr>
            
            <!-- DADOS DA SECRETARIA -->
            <td rowspan="9" class="SubTituloDireita">Secretaria de Educação:</td>
            
            <tr>
                <td class="SubTituloDireita">Telefone:</td>
                <td>( <? echo campo_texto('secatelefoneddd', "N", $habilita, "DDD Telefone", 4, 2, "###", "", '', '', 0, 'id="secatelefoneddd"', '', substr($usuarioprojovem['secatelefone'], 0, 2)); ?> ) <? echo campo_texto('secatelefone', "S", $habilita, "Telefone", 17, 16, "#####-####", "", '', '', 0, 'id="secatelefone"', '', mascaraglobal($tel, "####-#####")); ?></td>
            </tr>
            
            <tr>
                <td class="SubTituloDireita">CEP:</td>
                <td><? echo campo_texto('secacep', "S", $habilita, "CEP", 10, 9, "#####-###", "", '', '', 0, 'id="secacep"', '', mascaraglobal($usuarioprojovem['secacep'], "#####-###"), 'getEnderecoPeloCEP(this.value);'); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Endereço</td>
                <td><? echo campo_texto('secaendereco', "S", $habilita, "CEP", 50, 200, "", "", '', '', 0, 'id="secaendereco"', '', $usuarioprojovem['secaendereco']); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Bairro:</td>
                <td><? echo campo_texto('secabairro', "S", $habilita, "Bairro", 50, 200, "", "", '', '', 0, 'id="secabairro"', '', $usuarioprojovem['secabairro']); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Complemento:</td>
                <td><? echo campo_texto('secacomplemento', "N", $habilita, "Complemento", 50, 200, "", "", '', '', 0, 'id="secacomplemento"', '', $usuarioprojovem['secacomplemento']); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Número:</td>
                <td><? echo campo_texto('secanumero', "S", $habilita, "CEP", 9, 10, "#########", "", '', '', 0, 'id="secanumero"', '', $usuarioprojovem['secanumero']); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita">UF:</td>
                <td><? echo campo_texto('secauf', "N", "N", "UF", 2, 4, "", "", '', '', 0, 'id="secauf"', '', $usuarioprojovem['secauf']); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Municipio:</td>
                <td>
                    <input type="hidden" name="secamuncod" id="secamuncod" value="<?= $usuarioprojovem['secamuncod'] ?>">
    				<? echo campo_texto('mundescricao', "N", "N", "Município", 50, 200, "", "", '', '', 0, 'id="mundescricao"', '', (($usuarioprojovem['secamuncod']) ? $db->pegaUm("SELECT mundescricao FROM territorios.municipio WHERE muncod='" . $usuarioprojovem['secamuncod'] . "'") : "")); ?>
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
    <?registarUltimoAcesso();?>
    
