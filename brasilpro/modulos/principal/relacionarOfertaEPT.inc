<?php

include_once( APPRAIZ."includes/classes/dateTime.inc" );
include_once( APPRAIZ."brasilpro/classes/EntidadeDetalhe.class.inc" );

cte_verificaSessao();

$obEntidaDedetalhe = new EntidadeDetalhe();

if( $_REQUEST["atualizar"] ){
	
	$arEntid = $_REQUEST["sbmEntid"];
	
	if( is_array( $arEntid ) ){
		
		foreach( $arEntid as $entid ){
	
			
			if( $_REQUEST['entdoferta_ept_'.$entid] === null ) continue;
			
			$sql = "SELECT count(*) FROM entidade.entidadedetalhe WHERE entid = " .$entid;
	        $cnt = (integer) $db->pegaUm($sql);
	        
	        $obData = new Data();
	        
	        $entdstatusept = isset( $_REQUEST["csStatusOferta_".$entid] ) ? "'{$_REQUEST["csStatusOferta_".$entid]}'" : 'null';
	        $entddataprevisaoept = $_REQUEST["entddataprevisaoept_".$entid] ? "'". $obData->formataData( $_REQUEST["entddataprevisaoept_".$entid], "YYYY-MM-DD" ) ."'" : 'null';
	        
	        if ($cnt == 1) {
	            $sql = '
	                UPDATE entidade.entidadedetalhe
	                SET
	                    entdoferta_ept 		= ' . $_REQUEST['entdoferta_ept_'.$entid] .',
	                    entdstatusept 		= ' . $entdstatusept .',
	                    entddataprevisaoept = ' . $entddataprevisaoept .' 
	                WHERE entid = ' . $entid;
	            
	        } 
			else {
	            $sql = 'SELECT entcodent
	            		FROM entidade.entidade
	            		WHERE entid = '. $entid;
	
	            $entcodent = $db->pegaUm($sql);
	
	            if ($entcodent) {
	                $sql = ' INSERT INTO entidade.entidadedetalhe
	                			( entid, entdoferta_ept, entdstatusept, entddataprevisaoept, entcodent ) 
								VALUES ( ' . $entid . ', 
	                    				 ' . $_REQUEST['entdoferta_ept_'.$entid] . ',
	                    				 ' . $entdstatusept . ',
	                    				 ' . $entddataprevisaoept . ', 
	                     				 \'' .trim($entcodent). '\')';
	            }
	        }
	        
	        $db->executar($sql);
	        
	        if( $_REQUEST['entdoferta_ept_'.$entid] == 'false' ){
	            $sql = 'SELECT ippid, ipptitulo
	            		FROM cte.itensppp
	            		WHERE ippbloqdifept = true';
	
	            $itens = (array) $db->carregar($sql);
	
	            if( $itens ){
	                foreach ($itens as $item) {
	                    $sql = "SELECT cppid FROM cte.conteudoppp WHERE ippid = '". $item['ippid'] ."' AND entid = '". $entid ."'";
	                    $cpp = current( ( array ) $db->carregar( $sql ) );
	
	                    if( $cpp['cppid'] ){
	                        $sql = "DELETE FROM cte.conteudopppcursotecnico WHERE cppid = '".$cpp['cppid']."'";
	                        $db->executar($sql);
	                        $sql = "DELETE FROM cte.conteudoppp WHERE cppid = '".$cpp['cppid']."'";
	                        $db->executar($sql);
	                    }
	                }
	            }
	        }
        }
		$db->commit();

		echo '<script type="text/javascript">
				alert("Formação EPT foi gravada com sucesso.");
				location.href = "?modulo=principal/estrutura_avaliacao&acao=A";
			  </script>';
		exit;
		
	}
}


$coEntidadeDetalhe = $obEntidaDedetalhe->recuperarEscolasAtendidas( $_SESSION["inuid"] );

$arAgrupadores = array();
foreach( $coEntidadeDetalhe as $arEntidadeDetalhe ){
	$arAgrupadores[$arEntidadeDetalhe["mundescricao"]][] = $arEntidadeDetalhe;
}

$coEntidadeDetalhe = $arAgrupadores;


include_once( APPRAIZ."includes/cabecalho.inc" );
print "<br>";

$obEntidaDedetalhe->cria_aba( $abacod_tela, $url, '' );

$estuf = cte_pegarEstuf( $_SESSION["inuid"] );
$estDescricao = cte_pegarEstdescricao( $estuf )." - ".$estuf;

monta_titulo( $estDescricao, $titulo_modulo );

?>

<script type="text/javascript">
	function informarOfertaEPT( entid ){
		var eptSim = document.getElementById('entdoferta_ept_sim_' + entid);
		var eptNao = document.getElementById('entdoferta_ept_nao_' + entid);
		var spanImagem = document.getElementById('spanImgStatus_' + entid);
		
		img = document.createElement( "img" );
		img.setAttribute( "src", "../imagens/unchecked.jpg" );
		
		if( eptSim.checked ){
			document.getElementById('statusOferta_' + entid).style.display = '';
		}
		else{
			if( eptNao.checked ){
				document.getElementById('statusOferta_' + entid).style.display = 'none';
				document.getElementById('funcionamento_' + entid).checked = false;
				document.getElementById('previsto_' + entid).checked = false;
				document.getElementById('divDtPrevisao_' + entid).style.display = 'none';
				document.getElementById('entddataprevisaoept_' + entid).value = "";
				img.setAttribute( "src", "../imagens/check.jpg" );
			}	
		}
		
		spanImagem.innerHTML = "";
		spanImagem.appendChild( img );
		
	}
	
	function informarStatusOferta( entid ){
	
		var eptSim = document.getElementById('entdoferta_ept_sim_' + entid);
		var funcionamento = document.getElementById('funcionamento_' + entid);
		var previsto = document.getElementById('previsto_' + entid);
		var spanImagem = document.getElementById('spanImgStatus_' + entid);
			
		img = document.createElement( "img" );
		img.setAttribute( "src", "../imagens/unchecked.jpg" );
			
		if( funcionamento.checked ){
			document.getElementById('divDtPrevisao_' + entid).style.display = 'none';
			document.getElementById('entddataprevisaoept_' + entid).value = "";
			img.setAttribute( "src", "../imagens/check.jpg" );
		}
		else{
			if( previsto.checked ){
				document.getElementById('divDtPrevisao_' + entid).style.display = '';
			}
		}
		
		if( eptSim.checked ){
			spanImagem.innerHTML = "";
			spanImagem.appendChild( img );
		}
	}
	
	function verificaErroPreenchimentoData( entid ){
	
		var dataPrevisao = document.getElementById('entddataprevisaoept_' + entid);
		var spanImagem = document.getElementById('spanImgStatus_' + entid);
		var eptSim = document.getElementById('entdoferta_ept_sim_' + entid);
		var previsto = document.getElementById('previsto_' + entid);		
		
		img = document.createElement( "img" );
	
		if( dataPrevisao.value ){
			if( !validaData( dataPrevisao ) ){
				img.setAttribute( "src", "../imagens/unchecked.jpg" );
				alert( "Data inválida.\n\rFavor informar uma data válida!." );
				dataPrevisao.value = "";
			}
			else{
				img.setAttribute( "src", "../imagens/check.jpg" );
			}
		}
		else{
			img.setAttribute( "src", "../imagens/unchecked.jpg" );
		}
		
		if( eptSim.checked && previsto.checked ){
			spanImagem.innerHTML = "";
			spanImagem.appendChild( img );
		}

	}
	
	function gravar_ofertas_ept(){
		document.getElementById("formulario").submit();
	}
	
	
</script>

<style type="text/css">

	table tr td{
		padding: 5px 3px;	
	}
	
	.aviso{
		color: #f00;
		text-align: center;
		padding: 10px;
		font-size: 15px;
	}

</style>




<div align="center">
	
	<div class="aviso">
		Favor preencher as informações sobre Oferta EPT de todas as escolas atendidas pelo Estado.<br />
		Faz-se necessário o preenchimento para prosseguir.
	</div>
	
	<form action="#" name="formulario" id="formulario" method="post">
		<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<?php foreach( $coEntidadeDetalhe as $stMunicipio => $entidadeDetalhe ){ ?>
		        
		        <tr style="padding:15px; background-color:#ccc; font-weight: bold; color:#404040; vertical-align: top;">
		            <td colspan="2" style="text-align: center; font-weight: bold; letter-spacing: 3px; text-transform: uppercase;" ><? echo $stMunicipio; ?></td>
		        </tr>
		            
				<?php foreach( $entidadeDetalhe as $arEntidadeDetalhe ){ 
				
					$stNomeCampo = "entddataprevisaoept_".$arEntidadeDetalhe['entid'];
					$$stNomeCampo = $arEntidadeDetalhe["entddataprevisaoept"]; 
					?>
	
			        <tr style="padding:15px; background-color:#ddd; font-weight: bold; color:#404040; vertical-align: top;">
			            <td colspan="2">
							<input type="hidden" name="atualizar" value="gravarformacaoept">
			            	<strong><? echo $arEntidadeDetalhe['entnome']; ?></strong>
			            </td>
			        </tr>    
			        <tr style="padding:15px; background-color:#fafafa; font-weight: bold; color:#404040; vertical-align: top;">
				        <td>
				        	<input type="hidden" name="sbmEntid[]" id="sbmEntid_<?php echo $arEntidadeDetalhe['entid'] ?>" value="<?php echo $arEntidadeDetalhe['entid'] ?>" />
				        	<span id="spanImgStatus_<?php echo $arEntidadeDetalhe['entid'] ?>"></span>
				        </td>
			            <td>
			            	Informe se a escola oferta ou ofertará EPT: 
			            	<input type="radio" name="entdoferta_ept_<?php echo $arEntidadeDetalhe['entid'] ?>" id="entdoferta_ept_sim_<?php echo $arEntidadeDetalhe['entid'] ?>" 
			            		onclick="informarOfertaEPT( '<?php echo $arEntidadeDetalhe['entid'] ?>' )" 
			            		<? echo (($arEntidadeDetalhe['entdoferta_ept'] == 't')?'checked':''); ?> value="true"> <strong>Sim</strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			            	<input type="radio" name="entdoferta_ept_<?php echo $arEntidadeDetalhe['entid'] ?>" id="entdoferta_ept_nao_<?php echo $arEntidadeDetalhe['entid'] ?>"
			            		onclick="informarOfertaEPT( '<?php echo $arEntidadeDetalhe['entid'] ?>' );" 
			            		<? echo (($arEntidadeDetalhe['entdoferta_ept'] == 'f')?'checked':''); ?> value="false"> <strong>Não</strong>
				            <span id="statusOferta_<?php echo $arEntidadeDetalhe['entid'] ?>" style="display: none;">
								&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ||| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;				            
								<span>
									Status da Oferta:
									<input <?php echo (($arEntidadeDetalhe['entdstatusept'] == 'F')?'checked':''); ?> type="radio" onclick="informarStatusOferta( '<?php echo $arEntidadeDetalhe['entid'] ?>' );" name="csStatusOferta_<?php echo $arEntidadeDetalhe['entid'] ?>" id="funcionamento_<?php echo $arEntidadeDetalhe['entid'] ?>" value="F" /><strong>Em Funcionamento</strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
									<input <?php echo (($arEntidadeDetalhe['entdstatusept'] == 'P')?'checked':''); ?> type="radio" onclick="informarStatusOferta( '<?php echo $arEntidadeDetalhe['entid'] ?>' );" name="csStatusOferta_<?php echo $arEntidadeDetalhe['entid'] ?>" id="previsto_<?php echo $arEntidadeDetalhe['entid'] ?>" value="P" /><strong>Previsto</strong> 
									<span id="divDtPrevisao_<?php echo $arEntidadeDetalhe['entid'] ?>" style="display: none;">
										&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ||| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;				            
										Data de previsão:
										<?php echo campo_data( 'entddataprevisaoept_'.$arEntidadeDetalhe['entid'],'N', 'S', '', 'S', '', "verificaErroPreenchimentoData( '".$arEntidadeDetalhe['entid']."' )" ); ?>
									</span>
								</span>				            
				            </span>
      			            <script type="text/javascript">
			            		informarOfertaEPT( '<?php echo $arEntidadeDetalhe['entid'] ?>' );
			            		informarStatusOferta( '<?php echo $arEntidadeDetalhe['entid'] ?>' );
			            		verificaErroPreenchimentoData( '<?php echo $arEntidadeDetalhe['entid'] ?>' );
			            	</script>
						</td>		            		
			        </tr>
				<?php } ?>	
			<?php } ?>	
		</table>
		<br />
		<input type='button' class="botao" value='Gravar' onclick="gravar_ofertas_ept()" />
	</form>
</div>