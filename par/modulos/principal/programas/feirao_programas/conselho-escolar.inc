<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
<script type="text/javascript">	
	jQuery.noConflict();
</script>
<?php

if($_POST['arr']){
	
	$arrDados = explode("&",$_REQUEST['arr']);
	$flag = false;
	if($arrDados[0]) {
		foreach($arrDados as $arri) {
			$ar = explode("=",$arri);
			if($ar[1]) {
				$start = (stripos($ar[0],"%")+3);
				$end = strripos($ar[0],"%");
				$sql = "UPDATE par.pfcursista
						SET pscid = ".$ar[1]." 
						WHERE pcuid = ".substr($ar[0],$start,($end-$start));
				$db->executar($sql);
				$flag = true;
			}
		}
	}
	
	if($flag) {
		$db->commit();
		unset($_POST['arr']);
		die("<script>
				alert('Operação efetuada com sucesso.');
				location.href = 'par.php?modulo=principal/programas/feirao_programas/conselho-escolar&acao=A&pfcid=".$_REQUEST['pfcid']."&numero=".$_REQUEST['numero']."';
			</script>");
	}
 }

if($_REQUEST['pfcid']){
	
	$sql = "select 
				funid,
				pfctitulo,
				pfclimiteregistros,
				pfcorientacoes,
				pfcscripts,
				pfcidsformacao,
				pfcidsfuncaoatual
			from
				par.pfcurso
			where 
				pfcid = ".$_REQUEST['pfcid'];
	
	$rs = $db->pegaLinha($sql);
	if($rs) extract($rs);
}

if(!$_SESSION['par']['adpid']){
	echo '<script type="text/javascript"> 
    		alert("Sessão Expirou.\nFavor selecione o programa novamente!");
    		window.location.href="par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa";
    	  </script>';
    die;
}

//verifica se ja existe cursista na mesma escola
if($_POST['verificaCursista']){
	
	$sql = "SELECT pcuid FROM par.pfcursista 
			WHERE adpid = ".$_SESSION['par']['adpid']."
			AND pfcid = ".$_POST['pfcid']."
			AND entid = ".$_POST['escola'];
	$pcuidX = $db->pegaUm($sql);
			
	if($pcuidX){
		echo "ok";		
	}
	else{
		echo "";
	}
	die;
}

//pega array de perfis
$arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);

//pega estado atual
$docid = pgCriarDocumento( $_SESSION['par']['adpid'] );
$esdid = prePegarEstadoAtual( $docid );


	
	require_once APPRAIZ . "includes/classes/entidades.class.inc";
	
	ini_set( "memory_limit", "1024M" );
	
	$obEntidade = new Entidades();
	$obPfCursista = new PfCursista();
	$obPfEntidade = new PfEntidade();
	
	$arPfCursista = $obPfCursista->recuperarPorAdpid($_SESSION['par']['adpid'], $_REQUEST['pcuid']);

	if ($_REQUEST['opt'] == 'excluir') {
	
		if($_REQUEST['pcuid']){
                    $obPfCursista->excluir($_REQUEST['pcuid']);
                    $obPfCursista->commit();

                    echo "
                        <script>
                            alert('Dados excluidos com sucesso.');
                            location.href = 'par.php?modulo=principal/programas/feirao_programas/conselho-escolar&acao=A&pfcid=".$_REQUEST['pfcid']."&numero=".$_REQUEST['numero']."';
                        </script>";
		}
		exit();
	}
	
	if ($_REQUEST['opt'] == 'salvarRegistro') {
		
		$pcucpf = str_replace(' ','',str_replace('/','',str_replace('-','',str_replace('.','',$_REQUEST['entnumcpfcnpj']))));
		$boEntid = $obPfCursista->verificaCursistaExiste($pcucpf, $_SESSION['par']['adpid'], FUNID_CURSISTA_PROGRAMA_FORMACAO, $_REQUEST['pfcid']);
		$boTutorCoordenador = $obPfCursista->verificaEntidadeCoordenadorTutorExiste($pcucpf, $_SESSION['par']['adpid']);

                $totalVagas = 1;
                $sql = "SELECT count(pcuid) as total
                                FROM par.pfcursista 
                                WHERE adpid = {$_SESSION['par']['adpid']}
                                AND pfcid = {$_REQUEST['pfcid']}";
                $totalCursista = $db->pegaUm($sql);
                if(!$totalCursista) $totalCursista = 0;

		if(!$boEntid && !$boTutorCoordenador || $_REQUEST['pcuid']){
			$arPfcid[] = $_REQUEST['pfcid'];

                if(!$_REQUEST['escola']) $_REQUEST['escola'] = true;
                        
                        # Verifica se Dados do formulário do Conselho Escolar estão preenchido
			if(
                            $pcucpf
                            && $_REQUEST['entnome']
                            && $_REQUEST['entemail']
                            && $_REQUEST['pcuorgao']
                            && $_REQUEST['pcucargoefetivo'] 
                            && $_REQUEST['pcuexerciciosecretariaeducacao']
                            && $_REQUEST['entnumdddcomercial']
                            && $_REQUEST['entnumcomercial']
                        ){

                            if($_REQUEST['pcucargoefetivo'] == 'f' && $_REQUEST['pcuexerciciosecretariaeducacao'] == 'f'){
                                echo "
                                    <script>
                                        alert('Conforme cláusula quinta do Termo de Adesão o indicado deverá ser um servidor da secretaria de educação.');
                                        document.location.href = 'par.php?modulo=principal/programas/feirao_programas/conselho-escolar&acao=A&pfcid={$_REQUEST['pfcid']}';
                                    </script>";
                                die;
                            }
                            
                            # Verifica se existe adesao no intervalo do prazo do programa
                            $validade = verificarPrazoExpirado();
                            if(!empty($validade)){
                                echo "
                                    <script>
                                        alert('$validade');
                                        document.location.href = 'par.php?modulo=principal/programas/feirao_programas/conselho-escolar&acao=A&pfcid={$_REQUEST['pfcid']}';
                                    </script>";
                                die;
                            }

				$resultadoEdicao = verificaModificacaoFormularioInterlocutor($esdid,$arrayPerfil);
                                if($resultadoEdicao){
                                    if(!$_REQUEST['pffid']) $_REQUEST['pffid'] = 14;
                                    if(!$_REQUEST['tvpid']) $_REQUEST['tvpid'] = 4;
                                    $obPfCursista->pcuid 				= $_REQUEST['pcuid'] ? $_REQUEST['pcuid'] : ($_SESSION['par']['pcuid'] ? $_SESSION['par']['pcuid'] : null);
                                    $obPfCursista->adpid 				= $_SESSION['par']['adpid'];
                                    $obPfCursista->pfcid 				= $_REQUEST['pfcid'];
                                    $obPfCursista->entid 				= $_REQUEST['escola'];
                                    $obPfCursista->pcucpf 				= $pcucpf;
                                    $obPfCursista->pcunome 				= $_REQUEST['entnome'];
                                    $obPfCursista->tvpid 				= $_REQUEST['tvpid'];
                                    $obPfCursista->tfoid 				= $_REQUEST['tfoid'];
                                    $obPfCursista->pffid 				= $_REQUEST['pffid'];
                                    $obPfCursista->pcuemail 			= $_REQUEST['entemail'];
                                    $obPfCursista->pcudddnumtelefone 	= $_REQUEST['entnumdddcomercial'];
                                    $obPfCursista->pcunumtelefone 		= str_replace(' ','',str_replace('/','',str_replace('.','',str_replace('-','',$_REQUEST['entnumcomercial']))));
                                    $obPfCursista->pcuramaltelefone = $_REQUEST['entnumramalcomercial'];
                                    $obPfCursista->pcufuncao 			= $_REQUEST['pffid'] == 14 ? $_REQUEST['pcufuncao'] : "";
                                    $obPfCursista->pcuvinculo 			= $_REQUEST['tvpid'] == 4 ? $_REQUEST['pcuvinculo'] : "";
                                    $obPfCursista->pcuano			 	= $_REQUEST['pcuano'];

                                    # PROG_PAR_ADESAO_CONSELHO_ESCOLAR
                                    $obPfCursista->pcuorgao 						= $_REQUEST['pcuorgao'];
                                    $obPfCursista->pcucargoefetivo 					= $_REQUEST['pcucargoefetivo'];
                                    $obPfCursista->pcuexerciciosecretariaeducacao 	= $_REQUEST['pcuexerciciosecretariaeducacao'];
                                    $obPfCursista->pcufasecurso 					= $_REQUEST['pcufasecurso'];

                                    $obPfCursista->salvar();

                                    $obPfCursista->commit();

                                    $sql = "select pfcmsgsucesso from par.pfcurso where pfcid = {$_REQUEST['pfcid']}";

                                    $pfcmsgsucesso = $db->pegaUm($sql);

                                    $msg = 'Dados gravados com sucesso.';
                                    if($pfcmsgsucesso){
                                            $msg = $pfcmsgsucesso;
                                    }

                                    echo "<style> .ui-dialog-buttonset, button{ font-size:12px; } .ui-dialog-titlebar{ font-size:14px; } .ui-dialog-titlebar-close{ display: none; }</style>";
                                    echo '<div id="dialog-confirm" title="Confirmação" style="display:none;font-size:12px;">
                                                    <div id="dialog-content"></div>
                                              </div>';				
                                    echo "<script>										
                                                    jQuery(function(){
                                                            jQuery( '#dialog-confirm' ).attr({'title':'Alerta'});				
                                                            jQuery( '#dialog:ui-dialog' ).dialog( 'destroy' );
                                                            jQuery( '#dialog-content' ).html('{$msg}');							
                                                            jQuery( '#dialog-confirm' ).dialog({
                                                                    resizable: false,
                                                                    width: 550,
                                                                    modal: true,
                                                                    buttons: {
                                                                            'Ok': function() {									
                                                                                    jQuery( this ).dialog( 'close' );
                                                                                    window.location.href='par.php?modulo=principal/programas/feirao_programas/conselho-escolar&acao=A&pfcid={$_REQUEST['pfcid']}';
                                                                            }
                                                                    }
                                                            });
                                                    });										
                                      </script>";
                            }else{
				
				echo "
                                    <script>
					alert('Não é possível modificar o cadastro de interlocutor!');
					document.location.href = 'par.php?modulo=principal/programas/feirao_programas/conselho-escolar&acao=A&pfcid={$_REQUEST['pfcid']}';
                                    </script>";
                            }
                            exit;
			} else {
                            echo "<script>
                                    alert('Preencha todos os campos.');
                                    document.location.href = 'par.php?modulo=principal/programas/feirao_programas/conselho-escolar&acao=A&pfcid={$_REQUEST['pfcid']}';
                              </script>";
                            exit;
			}
	
		}else{
			$stTipo = $boTutorCoordenador['funid'] == 87 ? "coordenador" : "professor tutor";
				echo "<script>
					alert('".($boTutorCoordenador['entid'] ? 'Este CPF pertence a um '.$stTipo.'.' : 'Cursista já cadastrado.')."');
					document.location.href = 'par.php?modulo=principal/programas/feirao_programas/conselho-escolar&acao=A&pfcid={$_REQUEST['pfcid']}';
				  </script>";
			exit;
	
		}
	}


include_once "termoprofuncionario.inc";

$aderiu = testaAdesaoProFuncionario();

if( $aderiu == 'S' ){
include_once APPRAIZ . 'includes/cabecalho.inc';

echo '<br>';
echo carregaAbasProFuncionario('par.php?modulo=principal/programas/feirao_programas/conselho-escolar&acao=A&pfcid='.$_REQUEST['pfcid']);

// Verifica se o titulo foi preenchido na tabela de cursos
if(!empty($pfctitulo)){
	$titulo_modulo = $pfctitulo; 
}

$titulo_modulo2 = '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica campo obrigatório.';
monta_titulo( 'Cadastro de Interlocutor com a Coordenação Estadual', $titulo_modulo2);

if($pfcorientacoes){
	echo '<table cellspacing="1" cellpadding="15" bgcolor="#f5f5f5" align="center" id="tblentidade" class="tabela">
			<tr>
				<td>'.$pfcorientacoes.'</td>
				<td width="150px;">&nbsp;</td>
			</tr>
		  </table>';	
}

        # PROG_PAR_CONSELHO_ESCOLAR
        $totalVagas = 1;
        
	$sql = "SELECT count(pcuid) as total
	   		FROM par.pfcursista 
	   		WHERE adpid = {$_SESSION['par']['adpid']}
	   		AND pfcid = {$_REQUEST['pfcid']}";
	$totalCursista = $db->pegaUm($sql);
	if(!$totalCursista) $totalCursista = 0;
	$totalVagasInseridas = $totalVagas - $totalCursista;
	$saldoVagas = $totalVagasInseridas;

?>
<script type="text/javascript">
	jQuery(document).ready(function(){
		jQuery('#btngravar').val('Salvar');
		jQuery('#btncancelar').val('Limpar');
	
		jQuery('#btncancelar').click(function(){
			jQuery('#frmEntidade').each(function(){
				for(x=0;x<this.length;x++){
					if(this[x].type != 'button' &&
					   this[x].type != 'submit' &&
					   this[x].id   != 'funid' &&
					   this[x].id   != 'entidassociado'){
	
						this[x].value = '';
					}
				}
			});
		});
	
		jQuery('select[name=pffid]').live('change', function(){
			if(this.value == 14){
				jQuery('#pcufuncao').show();
				jQuery('#opcfuncao').show();
				this.hide();
			}
		});
	
		jQuery('select[name=tvpid]').live('change', function(){
			if(this.value == 4){
				jQuery('#pcuvinculo').show();
				jQuery('#opcvinculo').show();
				this.hide();
			}
		});
	
		jQuery('.mostraOpcoes').live('click', function(){
	
			if(this.id == 'opcfuncao'){
	
				this.hide();
				jQuery('#pcufuncao').hide();
				jQuery('select[name=pffid]').show();
				jQuery('select[name=pffid]').val('');
	
			} else if(this.id == 'opcvinculo'){
	
				this.hide();
				jQuery('#pcuvinculo').hide();
				jQuery('select[name=tvpid]').show();
				jQuery('select[name=tvpid]').val('');
			}
		});
	
		var cpFormacao 	 = jQuery('#campoFormacao');
		var cpVinculo 	 = jQuery('#campoVinculo');
		var cpFuncao 	 = jQuery('#campoFuncao');
		var campoEscolas = jQuery('#campoEscolas');

		// PROG_PAR_CONSELHO_ESCOLAR
                var cpOrgao		 	 = jQuery('#campoOrgao');
                var cpCargoEfetivo 	 = jQuery('#campoCargoEfetivo');
                var cpExercicio 	 = jQuery('#campoExercicio');
                var cpCursoFase 	 = jQuery('#campoCursoFase');

	
		jQuery('#tr_entnumdddcomercial').find('td:first').html('Telefone :');
		jQuery('#tr_entobs').after('<tr id="tr_formacao"><td class="SubTituloDireita" width="30%">Formação (Escolaridade):</td><td>'+cpFormacao.html()+'</td></tr>');
		jQuery('#tr_formacao').after('<tr id="tr_vinculo"><td class="SubTituloDireita">Vinculo :</td><td>'+cpVinculo.html()+'</td></tr>');
		jQuery('#tr_vinculo').after('<tr id="tr_funcao"><td class="SubTituloDireita">Função atual:</td><td>'+cpFuncao.html()+'</td></tr>');
		
		// PROG_PAR_CONSELHO_ESCOLAR
                jQuery('#tr_funcao').after('<tr id="tr_orgao"><td class="SubTituloDireita">Órgão:</td><td>'+cpOrgao.html()+'</td></tr>');
                jQuery('#tr_orgao').after('<tr id="tr_cargoefetivo"><td class="SubTituloDireita">É cargo efetivo da Secretaria de Educação?:</td><td>'+cpCargoEfetivo.html()+'</td></tr>');
                jQuery('#tr_cargoefetivo').after('<tr id="tr_exercicio"><td class="SubTituloDireita">Está em exercício na Secretaria de Educação?:</td><td>'+cpExercicio.html()+'</td></tr>');
                
                jQuery('#tr_titulo').hide();
                jQuery('#tr_acoes').hide();
                jQuery('#tr_exercicio').after('<tr id="tr_acoes" valign="middle" height="80px"><td class="SubTituloCentro" colspan="2"><input id="btngravar" type="submit" value="Salvar"><input id="btncancelar" type="reset" value="Limpar"></td></tr>');

		jQuery('#tr_acoes').after('<tr id="tr_hidden"><td colspan="2"><input type="hidden" name="pcuid" value="<?php echo $arPfCursista['pcuid'] ?>" /></td></tr>');
	
		cpFormacao.remove();
		cpVinculo.remove();
		cpFuncao.remove();
		campoEscolas.remove();
	
		jQuery('#tr_funcoescadastradas').hide();
		jQuery('#tr_entcodent').hide();
		jQuery('#tr_entunicod').hide();
		jQuery('#tr_entungcod').hide();
		jQuery('#tr_njuid').hide();
		jQuery('#tr_tpctgid').hide();
		jQuery('#tr_tpcid').hide();
		jQuery('#tr_tplid').hide();
		jQuery('#tr_tpsid').hide();
	
		jQuery('#tr_entnumdddresidencial').hide();
		jQuery('#tr_entdatanasc').hide();
		jQuery('#tr_entorgaoexpedidor').hide();
		jQuery('#tr_entnumrg').hide();
		jQuery('#tr_entsexo').hide();
		jQuery('#tr_entnumdddfax').hide();
		jQuery('#tr_entnumdddcelular').hide();
	
		jQuery('#tr_endereco').hide();
		jQuery('#tr_endcom').hide();
		jQuery('#tr_endnum').hide();
		jQuery('#tr_endbai').hide();
		jQuery('#tr_estuf').hide();
		jQuery('#tr_mundescricao').hide();
		jQuery('#tr_latitude').hide();
		jQuery('#tr_longitude').hide();
		jQuery('#tr_endmapa').hide();
	
		jQuery('#tr_endlog').hide();
		jQuery('#tr_endcep').hide();
		jQuery('#tr_funid').hide();
		jQuery('#tr_entobs').hide();
		
		jQuery('#entnumcomercial').after('&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');
		jQuery('#entemail').after('&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');
		jQuery('#entnome').after('&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');
		jQuery('#entnumcpfcnpj').after('&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');
	
		jQuery('#frmEntidade').submit(function(){
			if(jQuery('#tr_entnumcpfcnpj').css('display') != 'none'){
				if(jQuery('#entnumcpfcnpj').val()  == ''){
					alert('O campo CPF é obrigatório.');
					jQuery('#entnumcpfcnpj').focus();
					return false;
				}
			}
	
			if(jQuery('#tr_entnome').css('display') != 'none'){
				if(jQuery('#entnome').val()  == ''){
					alert('O campo Nome é obrigatório.');
					jQuery('#entnome').focus();
					return false;
				}
			}
	
			if(jQuery('#tr_entemail').css('display') != 'none'){
				if(jQuery('#entemail').val()  == ''){
					alert('O campo E-mail é obrigatório.');
					jQuery('#entemail').focus();
					return false;
				}
			}
			
			if(jQuery('#tr_entemail').css('display') != 'none'){
				var cemail = jQuery('#entemail').val();
				if(cemail.indexOf('@') == -1){
					alert('E-mail inválido.');
					jQuery('#entemail').focus();
					return false;
				}
			}

			if(jQuery('#tr_entnumdddcomercial').css('display') != 'none'){
				if(jQuery('#entnumdddcomercial').val()  == ''){
					alert('O campo DDD é obrigatório.');
					jQuery('#entnumdddcomercial').focus();
					return false;
				}
	
				if(jQuery('#entnumcomercial').val()  == ''){
					alert('O campo Telefone é obrigatório.');
					jQuery('#entnumcomercial').focus();
					return false;
				}
			}
	
			if(jQuery('select[name=tfoid]').val()  == ''){
				alert('O campo Formação é obrigatório.');
				jQuery('select[name=tfoid]').focus();
				return false;
			}
	
			if(jQuery('select[name=tvpid]').css('display') != 'none'){
				if(jQuery('select[name=tvpid]').val()  == ''){
					alert('O campo Vínculo é obrigatório.');
					jQuery('select[name=tvpid]').focus();
					return false;
				}
			}
	
			if(jQuery('#pcuvinculo').css('display') != 'none'){
				if(jQuery('#pcuvinculo').val()  == ''){
					alert('O campo Vínculo é obrigatório.');
					jQuery('#pcuvinculo').focus();
					return false;
				}
			}
	
			if(jQuery('select[name=pffid]').css('display') != 'none'){
				if(jQuery('select[name=pffid]').val()  == ''){
					alert('O campo Função é obrigatório.');
					jQuery('select[name=pffid]').focus();
					return false;
				}
			}
	
			if(jQuery('#pcufuncao').css('display') != 'none'){
				if(jQuery('#pcufuncao').val()  == ''){
					alert('O campo Função é obrigatório.');
					jQuery('#pcufuncao').focus();
					return false;
				}
			}

			if(jQuery('select[name=escola]').css('display') != 'none'){
				if(jQuery('select[name=escola]').val() == ''){
					alert('O campo Escola é obrigatório.');
					jQuery('select[name=escola]').focus();
					return false;
				}
			}

			// PROG_PAR_CONSELHO_ESCOLAR
                        if(jQuery('#pcuorgao').val() == ''){
                                alert('O campo Órgão é obrigatório.');
                                jQuery('#pcuorgao').focus();
                                return false;
                        }

                        if(!jQuery('input:radio[name=pcucargoefetivo]:checked').val()){
                                alert('O campo Cargo Efetivo é obrigatório.');
                                jQuery('[name=pcucargoefetivo]:checked').focus();
                                return false;
                        }

                        if(!jQuery('input:radio[name=pcuexerciciosecretariaeducacao]:checked').val()){
                                alert('O campo Exercício é obrigatório.');
                                jQuery('[name=pcuexerciciosecretariaeducacao]:checked').focus();
                                return false;
                        }
                        
                        // Regra para não permitir o sistema salvar os dados caso o usuario marque Nao para as opções de cargo e exercicio
                        if(jQuery('input:radio[name=pcucargoefetivo]:checked').val() == 'f' && jQuery('input:radio[name=pcuexerciciosecretariaeducacao]:checked').val() == 'f'){
                            alert('Conforme cláusula quinta do Termo de Adesão o indicado deverá ser um servidor da secretaria de educação.');
                            jQuery('[name=pcuexerciciosecretariaeducacao]:checked').focus();
                            return false;
                        }

		});
	
		if(jQuery('#tr_entnumcomercial').css('display') != 'none'){
			jQuery('#entnumcomercial').blur(function(){
				numero = this.value.replace('-','');
				if(numero.length < 8 && numero != ''){
					alert('Telefone inválido.');
					jQuery('#entnumcomercial').val('');
					jQuery('#entnumcomercial').focus();
					return false;
				}
			});
	
			jQuery('#entnumdddcomercial').blur(function(){
				numero = this.value.replace('-','');
				if(numero.length < 2 && numero != ''){
					alert('DDD inválido.');
					jQuery('#entnumdddcomercial').val('');
					jQuery('#entnumdddcomercial').focus();
					return false;
				}
			});
		}
	
		jQuery('.alterar').click(function(){
			var pfcid = "<?=$_REQUEST['pfcid']?>";
			document.location.href = '?modulo=principal/programas/feirao_programas/conselho-escolar&acao=A&pfcid='+pfcid+'&pcuid='+this.id;
		});
	
		jQuery('.excluir').click(function(){
			if(confirm('Deseja realmente excluir este registro?')){
				var numero = "<?=$_REQUEST['numero']?>";
				var pfcid = "<?=$_REQUEST['pfcid']?>";
				document.location.href = '?modulo=principal/programas/feirao_programas/conselho-escolar&acao=A&numero='+numero+'&pfcid='+pfcid+'&opt=excluir&pcuid='+this.id;
			}
		});
	
		<?php if($_REQUEST['pcuid']):?>
			jQuery('#entnumcpfcnpj').val("<?php echo $arPfCursista['pcucpf']?>");
			jQuery('#entnome').val("<?php echo $arPfCursista['pcunome']?>");
			jQuery('#entemail').val("<?php echo $arPfCursista['pcuemail']?>");
			jQuery('#entnumdddcomercial').val("<?php echo $arPfCursista['pcudddnumtelefone']?>");
			jQuery('#entnumcomercial').val("<?php echo $arPfCursista['pcunumtelefone']?>");
		<?php endif;?>

		jQuery('#tr_acoes').attr({'height':'80px','valign':'middle'});
	
                carregarFormulario();
                controlarBotoes();
	});

	</script>	
	<?php
	
	if(empty($funid)){
		$funid = FUNID_CURSISTA_PROGRAMA_FORMACAO;
	}
	
	$funentassoc = $funentassoc ? $funentassoc : null;

	echo $obEntidade->formEntidade("par.php?modulo=principal/programas/feirao_programas/conselho-escolar&acao=A&pfcid={$_REQUEST['pfcid']}&opt=salvarRegistro",
                                                                array("funid" => $funid, "entidassociado" => $funentassoc),
								 array( "enderecos"=>array(1) ) );

        $sql = "
            SELECT
                *
            FROM
                par.pfcursista
            WHERE
                pfcid = ".$_REQUEST['pfcid']."
            AND
                adpid = ".$_SESSION['par']['adpid'];

        $rsCursista = $db->pegaLinha($sql);

        if($rsCursista){
                $rsCursista['pcucpf'] = formatar_cpf($rsCursista['pcucpf']);

                unset($_SESSION['par']['pcuid']);
                if($rsCursista['pcuid']){
                        $_SESSION['par']['pcuid'] = $rsCursista['pcuid'];
                }
                
                if($rsCursista['pcucargoefetivo']=='t'){
                    $chkcargoEfetivo = "jQuery('input[name=pcucargoefetivo][value=t]').attr('checked','checked');";
                } else {
                    $chkcargoEfetivo = "jQuery('input[name=pcucargoefetivo][value=f]').attr('checked','checked');";
                }
                if($rsCursista['pcuexerciciosecretariaeducacao']=='t'){
                    $chkCargoExercicioSecretaria = "jQuery('input[name=pcuexerciciosecretariaeducacao][value=t]').attr('checked','checked');";
                } else {
                    $chkCargoExercicioSecretaria = "jQuery('input[name=pcuexerciciosecretariaeducacao][value=f]').attr('checked','checked');";
                }

                $jsCarregarFormulario = "
                    jQuery('[name=entemail]').val('{$rsCursista['pcuemail']}');
                    jQuery('[name=entnumdddcomercial]').val('{$rsCursista['pcudddnumtelefone']}');
                    jQuery('[name=entnumcomercial]').val('{$rsCursista['pcunumtelefone']}');
                    jQuery('[name=entnome]').val('{$rsCursista['pcunome']}');
                    jQuery('[name=entnumcpfcnpj]').val('{$rsCursista['pcucpf']}');
                    jQuery('[name=tfoid]').val('{$rsCursista['tfoid']}');						
                    jQuery('[name=tvpid]').val('{$rsCursista['tvpid']}');
                    jQuery('[name=pffid]').val('{$rsCursista['pffid']}');
                    jQuery('[name=pcuid]').val('{$rsCursista['pcuid']}');
                    jQuery('[name=escola]').val('{$rsCursista['entid']}');
                    jQuery('[name=pcuorgao]').val('{$rsCursista['pcuorgao']}');
                    $chkcargoEfetivo
                    $chkCargoExercicioSecretaria
                    jQuery('[name=entnumramalcomercial]').val('{$rsCursista['pcuramaltelefone']}');
                ";
        }

        if($pfcscripts){
                echo $pfcscripts;
        }
	
	$dados_wf = array('adpid' => $_SESSION['par']['adpid']);
	
	echo '<div style="position: absolute;top:395px; right: 80px;">';
	wf_desenhaBarraNavegacao($docid, $dados_wf);
	echo '</div>';
}else{
	
	echo "<table border=0 cellpading=0 cellspacing=0 width='100%'>
			<tr><td width='90%'>&nbsp;";
	echo "</td>";
	echo "<td width='10%'>";	
	
	$dados_wf = array('adpid' => $_SESSION['par']['adpid']);
	wf_desenhaBarraNavegacao($docid, $dados_wf);
	
	echo "</td></tr></table>";
}

echo "<form name='formulario' method='post' action=>";
echo "<input type=hidden name=arr id=arr>";
echo "<input type=hidden name=numero id=numero>";

    $botoes = "'<img src=\"../imagens/excluir.gif\" alt=\"Excluir\" title=\"Excluir\" class=\"excluir\" id=\"' || pcu.pcuid || '\" style=\"cursor:pointer\" />
                <img src=\"../imagens/alterar.gif\" alt=\"Alterar\" title=\"Alterar\" class=\"alterar\" id=\"' || pcu.pcuid || '\" style=\"cursor:pointer\"/>' as acao,";


		
    //monta combo query
    $sqlc = "select pscid, pscdescricao from par.pfsituacaocursista 
                            where prgid = ".$_SESSION['par']['prgid']."
                            and esdid = ".$esdid."
                            and pscstatus = 'A'";
    $dadoscombo = $db->carregar($sqlc);
    if($dadoscombo){
            $combo = '';
            foreach($dadoscombo as $d){
                    $combo .= '<option value='.$d['pscid'].'>'.$d['pscdescricao'].'</option>'; 
            }
    }

    $resultadoEdicao = verificaModificacaoFormularioInterlocutor($esdid,$arrayPerfil);
    if(!$resultadoEdicao){
        $botoes = "";
    }
    $sql = "
        SELECT DISTINCT
            $botoes
            pcu.pcucpf,
            pcu.pcunome,
            pcu.pcuemail,
            '(' || pcu.pcudddnumtelefone || ') ' || pcu.pcunumtelefone as telefone,
            tfo.tfodsc,
            CASE WHEN pcu.tvpid = 4 THEN pcu.pcuvinculo ELSE pfv.tvpdsc END AS tvpdsc,
            CASE WHEN pcu.pffid = 14 then pcu.pcufuncao ELSE pff.pffdescricao END AS pffdescricao,
            pcu.pcuorgao,
            CASE WHEN pcu.pcucargoefetivo = 't' THEN 'Sim' ELSE 'Não' END AS pcucargoefetivo,
            CASE WHEN pcu.pcuexerciciosecretariaeducacao = 't' THEN 'Sim' ELSE 'Não' END AS pcuexerciciosecretariaeducacao,
            'Fase ' || pcu.pcufasecurso AS pcufasecurso
        FROM par.pfcursista pcu
            INNER JOIN public.tipovinculoprofissional	pfv ON pfv.tvpid = pcu.tvpid
            INNER JOIN par.pffuncao						pff ON pff.pffid = pcu.pffid
            LEFT JOIN public.tipoformacao				tfo ON tfo.tfoid = pcu.tfoid
            LEFT JOIN entidade.entidade 				ent ON ent.entid = pcu.entid
            LEFT JOIN entidade.endereco 				eed ON eed.entid = ent.entid
            LEFT JOIN par.pfsituacaocursista			pfc ON pfc.pscid = pcu.pscid
        WHERE pcu.adpid = {$_SESSION['par']['adpid']}
            AND pfcid = {$_REQUEST['pfcid']}
            ORDER BY 3";

    if(empty($botoes)){
        $cabecalho = array("CPF", "Nome", "Email", "Telefone", "Formação", "Vínculo", "Função atual", "Órgão", "Cargo Efetivo", "Exercício", "Fase");
    } else {
        $cabecalho = array( "Ação", "CPF", "Nome", "Email", "Telefone", "Formação", "Vínculo", "Função atual", "Órgão", "Cargo Efetivo", "Exercício", "Fase");
    }

?>
</form>
<?php 
    if($pfclimiteregistros > 1 || empty($pfclimiteregistros)){
        $db->monta_lista($sql,$cabecalho,50,5,'N','95%',"","formdados");
    }
?>

	<div id="campoFormacao" style="display:none;">
		<?php
		
		$stWhere = '';
		if($pfcidsformacao){
			$stWhere = ' and tfoid in ('.$pfcidsformacao.') ';
		}
		
		// PROG_PAR_CONSELHO_ESCOLAR
                $stWhereCE = "AND tfoid NOT IN(5,6,7)";
		
		$sql = "SELECT	tfoid as codigo,
						tfodsc as descricao
				FROM 	public.tipoformacao
				WHERE 	tfostatus = 'A'
						{$stWhere}
						{$stWhereCE}
				ORDER BY tfodsc";
						
		unset($stWhere);
	
		$db->monta_combo('tfoid', $sql, 'S', 'Selecione uma formação...', null, null, null, 200, 'S', '', '', $arPfCursista['tfoid']);
		?>
	</div>
	<div id="campoVinculo" style="display:none;">
		<?php
		$sql = "SELECT	 tvpid as codigo,
						 tvpdsc as descricao
				FROM 	 public.tipovinculoprofissional tvp
				WHERE 	 tvpstatus = 'A'
				ORDER BY tvpdsc";
	
		$db->monta_combo('tvpid', $sql, 'S', 'Selecione um vínculo...', null, null, null, 200, 'N', '', '', $arPfCursista['tvpid']);
		?>
		<input type="text" class="normal obrigatorio" id="pcuvinculo" name="pcuvinculo" style="width: 50ex; display: none;" value="<?php echo $arPfCursista['pcuvinculo']?>" />
		<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
		<a href="javascript:void(0)" class="mostraOpcoes" id="opcvinculo" style="display:none;">Mostrar opções</a>
	</div>
	<div id="campoFuncao" style="display:none;">
		<?php
		
		$stWhere = '';
		if($pfcidsfuncaoatual){
			$stWhere = ' and pffid in ('.$pfcidsfuncaoatual.') ';
		}
		
		// PROG_PAR_CONSELHO_ESCOLAR
                $stWhereCE = "AND pffid NOT IN(10,15)";
		
		$sql = "SELECT	 pffid as codigo,
                                    pffdescricao as descricao
                   FROM 	 par.pffuncao
                   WHERE 	 pffstatus = 'A'
                   {$stWhere}
                   {$stWhereCE}
                   ORDER BY pffdescricao";
				
		unset($stWhere);
		
		$db->monta_combo('pffid', $sql, 'S', 'Selecione uma funcao...', null, null, null, 200, 'N', '', '', $arPfCursista['pffid']);
		?>
		<input type="text" class="normal obrigatorio" id="pcufuncao" name="pcufuncao" style="width: 50ex; display: none;" value="<?php echo $arPfCursista['pcufuncao']?>" />
		<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
		<a href="javascript:void(0)" class="mostraOpcoes" style="display:none;" id="opcfuncao">Mostrar opções</a>
	</div>

	<div id="campoOrgao" style="display:none;">
		<input type="text" class="normal obrigatorio" id="pcuorgao" name="pcuorgao" value="<?php echo $arPfCursista['pcuorgao']; ?>" />&nbsp;
		<img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
	</div>

	<div id="campoCargoEfetivo" style="display:none;">
		<input type="radio" name="pcucargoefetivo" value="t" <?php echo $arPfCursista['pcucargoefetivo'] == 't' ? 'checked="checked"' : '' ?>/>&nbsp;Sim&nbsp;
		<input type="radio" name="pcucargoefetivo" value="f" <?php echo $arPfCursista['pcucargoefetivo'] == 'f' ? 'checked="checked"' : '' ?> />&nbsp;Não&nbsp;
		<img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
	</div>

	<div id="campoExercicio" style="display:none;">
		<input type="radio" name="pcuexerciciosecretariaeducacao" value="t" <?php echo $arPfCursista['pcuexerciciosecretariaeducacao'] == 't' ? 'checked="checked"' : '' ?>/>&nbsp;Sim&nbsp;
		<input type="radio" name="pcuexerciciosecretariaeducacao" value="f" <?php echo $arPfCursista['pcuexerciciosecretariaeducacao'] == 'f' ? 'checked="checked"' : '' ?> />&nbsp;Não&nbsp;
		<img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
	</div>

<?php
$programa = pegarProgramaDisponivel();
$perfil = pegaArrayPerfil($_SESSION['usucpf']);

$programa = $programa ? $programa : array();
?>
<script>
    function controlarBotoes(){
        <?php
            # NOVA REGRA ADICIONA - SE O PROGRAMA ESTIVER COM A DATA DE ADESAO DISPONIVEL NÃO ENTRA E OS BOTÕES CONTINUAM VISIVEIS.
            # 1145 Estado: Em Cadastramento
            $resultadoEdicao = verificaModificacaoFormularioInterlocutor($esdid,$arrayPerfil);
            if(!$resultadoEdicao){
                echo "jQuery('#btngravar').hide();";
                echo "jQuery('#btncancelar').hide();";
                echo "desabilitarAlteracaoFormulario();";
            }
        ?>
    }

    function salvarSit(){
        var numero = "<?=$_REQUEST['numero']?>";
        //alert(jQuery('#formdados').serialize());
        jQuery('#arr').val(jQuery('#formdados').serialize());
        jQuery('#numero').val(numero);
        document.formulario.submit();
    }

    function selecionaFase(){
        if(jQuery('[name=pcufasecurso]').val() == 2){
            alert('Para cursar a Fase 2 do Curso é necessário que o Profissional da Educação tenha concluído a Fase 1, com aproveitamento');
        }
    }
    
    function desabilitarAlteracaoFormulario(){
        jQuery("#frmEntidade input, #frmEntidade select").attr('disabled', 'disabled');
    }

    function carregarFormulario(){
        <?php echo $jsCarregarFormulario?$jsCarregarFormulario:NULL; ?>
    }
</script>

<div id="dialog-confirm" title="Confirmação" style="display:none;">
	<div id="dialog-content"></div>
</div>
