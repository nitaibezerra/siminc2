<?php
// empreendimento || obra || orgao
verificaSessao( 'orgao' );

$orgid 		  = $_SESSION['obras2']['orgid'];
$obridSession = $_SESSION['obras2']['obrid'];

if (isset($_GET['fase']) == false){
	$_SESSION['obras2']['idFaseLicitacao'] = array();
}

if ( $_GET['acao'] == 'E' && $_GET['licid'] ){
	$licid = $_GET['licid'];
	$_SESSION['obras2']['licid'] = $licid;
}elseif ( $_GET['acao'] == 'E' ){
	$licid = $_SESSION['obras2']['licid'];
}elseif ( $_POST['licid'] ){
	$licid = $_POST['licid'];
}else{
	unset( $_SESSION['obras2']['licid'] );
}

$licitacao = new Licitacao( $licid );
if ( $licitacao->licid ){
	extract( $licitacao->getDados() );
}
switch ( $_REQUEST['requisicao'] ){
	case 'salvar':
		$licid = $licitacao->popularDadosObjeto( $_POST )
				  		   ->salvar();
		$db->commit();				  		   
		$_SESSION['obras2']['licid'] = $licid;	

		die("<script>
				alert('Operação realizada com sucesso!');" . 
				($obridSession
					? "window.location = '?modulo=principal/exibeLicitacao&acao=A';"
					: "window.location = '?modulo=principal/cadLicitacao&acao=E';") . "
			 </script>");
				  
		break;
	case 'excluirfase':
		$faseLicitacao = new FaseLicitacao( $_POST['flcid'] );
		$faseLicitacao->flcstatus = 'I';
		$faseLicitacao->salvar();
		$db->commit();				  		   
		exit;
	case "download":
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$arqid = $_REQUEST['arqid'];
		$file = new FilesSimec();
	    $arquivo = $file->getDownloadArquivo($arqid);
		die();
}
//demanda 311298 - perfil call_center não pode editar.
if( possui_perfil(PFLCOD_CALL_CENTER) ){
    $somenteLeitura = 'N';
    $desabilita     = "disabled = 'disabled';";
}
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
monta_titulo($titulo_modulo, '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.');

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<form method="post" id="formulario" name="formulario">
<input type="hidden" name="requisicao" id="requisicao" value="salvar"/>
<input type="hidden" name="licid" id="licid" value="<?php echo $licid ?>"/>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	<tr>
		<td width="265" class="subtitulodireita">Órgão:</td>
		<td>
			<?php 
			$orgao = new Orgao();
			echo $orgao->pegaDescricao( $orgid );
			?>
			<input type="hidden" name="orgid" id="orgid" value="<?php echo $orgid; ?>">
		</td>
	</tr>
	<tr>
		<td width="265" class="subtitulodireita">Modalidade de Licitação:</td>
		<td>
			<?php 
				$modalidade = new ModalidadeLicitacao();
				$db->monta_combo('molid', $modalidade->listaCombo(), 'S', "Selecione...", 'abreNumLicUasg', '', '', '', 'S', 'molid'); 
			?>
		</td>
	</tr>
	<tr>
		<td>Fases de Licitação</td>
	</tr>
	<tr>
		<td colspan="2">
			<table id="faseslicitacao" width="70%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
				<thead>
					<tr bgcolor="#e9e9e9">
						<td width="10%" valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
						<td width="70%" valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Descrição</strong></td>
						<td width="20%" valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Data</strong></td>						
					</tr>
				</thead>
				<?php 
					if($licid || $_GET['fase']){
						
						$faseLic = new FaseLicitacao();
						$arDados = $faseLic->listaPorLicitacao( $licid, ($_GET['fase'] ? $_SESSION['obras2']['idFaseLicitacao'] : array()) );

						foreach ($arDados as $k => $dados){
//						while (($dados = pg_fetch_assoc($sql))){
							$flcid 				= $dados['flcid'];
							$tflid 				= $dados['tflid'];
							$tfldesc 			= $dados['tfldesc'];
							$flcrecintermotivo 	= $dados['flcrecintermotivo'];
							$flcordservnum 		= $dados['flcordservnum'];
							$flcpubleditaldtprev = formata_data($dados['flcpubleditaldtprev']);
							$flcdtrecintermotivo = formata_data($dados['flcdtrecintermotivo']);
							$flcordservdt 		= formata_data($dados['flcordservdt']);
							$flchomlicdtprev 	= formata_data($dados['flchomlicdtprev']);
							$flcaberpropdtprev 	= formata_data($dados['flcaberpropdtprev']);
							$tflordem 			= $dados['tflordem'];
							$flcmeiopublichomol = $dados['flcmeiopublichomol'];
							$flcobshomol 		= $dados['flcobshomol'];
							$arqid		 		= $dados['arqid'];
							$arqnome	 		= $dados['arqnome'];
							$arqextensao 		= $dados['arqextensao'];
							
							if($tflid ==2){
								$_SESSION["obras2"]["flcpubleditaldtprev"] = formata_data_sql($flcpubleditaldtprev);
								$flcdata = $flcpubleditaldtprev;
							}
							if($tflid ==5){
								$flcdata = $flcdtrecintermotivo;
							}
							if($tflid ==6){
								$flcdata = $flcordservdt;
							}
							if($tflid ==9){
								$flcdata = $flchomlicdtprev;
								$_SESSION["obras2"]["flchomlicdtprev"] = $flchomlicdtprev; 
							}
							if($tflid ==7){
								$flcdata = $flcaberpropdtprev;
							}
							
							
							if($somenteLeitura == "N"){
								$botoes = "<img src=\"/imagens/excluir_01.gif\" style=\"cursor: pointer\"  border=0 title=\"Excluir\">";
							}else if(!empty($desabilita)){
                                                            $botoes = " <img src='/imagens/alterar.gif' style='cursor: pointer' border=0 title='Editar' onclick=\"atualizaFase(".$flcid.",'update');\"/>
									<img src=\"/imagens/excluir_01.gif\" style=\"cursor: pointer\"  border=0 title=\"Excluir\">
								      ";
								if ( $arqid ){
									$botoes .= "&nbsp;<img src='/imagens/anexo.gif' style='cursor: pointer' border=0 title='Baixar arquivo' onclick=\"DownloadArquivo(" . $arqid . ");\"/>";
								}else{
									$botoes .= "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
								}
                                                        }else{
								$botoes = " <img src='/imagens/alterar.gif' style='cursor: pointer' border=0 title='Editar' onclick=\"atualizaFase(".$flcid.",'update');\"/>
											<img src=\"/imagens/excluir.gif\" style=\"cursor: pointer\"  border=0 title=\"Excluir\" onClick=\"RemoveLinha(window.document.getElementById('tr_".$flcid."').rowIndex,'".$flcid."');\">
											";
								if ( $arqid ){
									$botoes .= "&nbsp;<img src='/imagens/anexo.gif' style='cursor: pointer' border=0 title='Baixar arquivo' onclick=\"DownloadArquivo(" . $arqid . ");\"/>";
								}else{
									$botoes .= "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
								}
							}
							
							echo "
								<tr id=\"tr_" . $flcid . "\">
									<td bgcolor=\"#F7F7F7\" align=\"center\">" . $botoes . "</td>
									<td><input type='hidden' name='flcid[]' id='flcid_".$flcid."'  value='".$flcid."'/>" . $tfldesc . "</td>
									<td>
									<input type='hidden' name='flcpubleditaldtprev[]' id='flcpubleditaldtprev_".$flcid."' value='".$flcpubleditaldtprev."'/>
									<input type='hidden' name='flcdtrecintermotivo[]' id='flcdtrecintermotivo_".$flcid."' value='".$flcdtrecintermotivo."'/>
									<input type='hidden' name='flcordservdt[]' id='flcordservdt_".$flcid."' value='".$flcordservdt."'/>
									<input type='hidden' name='flchomlicdtprev[]' id='flchomlicdtprev_".$flcid."' value='".$flchomlicdtprev."'/>
									<input type='hidden' name='flcaberpropdtprev[]' id='flcaberpropdtprev_".$flcid."' value='".$flcaberpropdtprev."'/>									
									<input type='hidden' name='flcdata[]' id='flcdata_".$flcid."' value='".$flcdata."'/>
									" . $flcdata . "
									<input type='hidden' name='flcrecintermotivo[]' id='flcrecintermotivo_".$flcid."' value='".$flcrecintermotivo."'/>
									<input type='hidden' name='flcobshomol[]' id='flcobshomol_".$flcid."' value='".$flcobshomol."'/>
									<input type='hidden' name='flcmeiopublichomol[]' id='flcmeiopublichomol_".$flcid."' value='".$flcmeiopublichomol."'/>
									<input type='hidden' name='flcordservnum[]' id='flcordservnum_".$flcid."' value='".$flcordservnum."'/>
									<input type='hidden' name='tflordem[]' id='flordem_".$flcid."' value='".$tflordem."'/>		
									<input type='hidden' name='tflid[]' id='tflid_".$flcid."' value='".$tflid."'/>
									</td>
								</tr>
							";
						}
					}
				
				?>
			</table>
			<div id='conteinerAcao'>			
			</div>
		</td>
	</tr>
        <?php if ( empty($desabilita) ):?>	
	<tr>
		<td>
			<a href="#" onclick="adicionarFases('licitacao');"><img src="/imagens/gif_inclui.gif" style="cursor:pointer;" border="0" title="Inserir Fases"> Inserir Fases de Licitação </a>
		</td>
	</tr>
        <?php endif; ?>
	<tr id="tr_numlic" style="display: none;">
		<td class="subtitulodireita">Número da Licitação:</td>
		<td>
			<?php 
				$licnumero = $licitacao->licnumero;
				echo campo_texto( 'licnumero', 'N', 'S', '', 12, 10, '##########', '', 'left', '', 0, 'id="licnumero"' ); 
			?>
		</td>
	</tr>
<?php 
	if ( empty($obridSession) ):	
?>
	<tr>
		<td class="SubTituloDireita" valign="top">Obra(s):</td>
		<td valign="top">
		<?
		$obra 		 = new Obras();
		$filtro 	 = array('naoLicitada'   => ($licid ? $licid : true), 
							 'orgid'	     => $orgid,
							 'not(obridpai)' => true);
		
		$sql 		 = $obra->listaComboPopupSql( $filtro );
		$obrid		 = ($obridSession && empty($licid) ? $obra->carregaComboPopupPorObra( $obridSession ) : $obra->carregaComboPopupPorLicitacao( $licid ));
		
		$filtro = array(
						array("codigo" 		=> "obrnome",
							  "descricao" 	=> "<b>Nome:</b>",
							  "string" 		=> "1")
						);
		combo_popup('obrid', $sql, 'Selecione a(s) Obra(s)', "400x400", 0, array(), "", "S", false, false, 4, 330, null, null, false, $filtro);
		?>
		</td>
	</tr>
<?php
	endif;
?>	
	<tr bgcolor="#DEDEDE">
		<td colspan="2">
		<?php
		if ( $obridSession ):
		?>
			<input type="hidden" name="obrid[]" id="obrid" value="<?php echo $obridSession; ?>"/>
                        <input type="button" name="botao" <?php echo $desabilita; ?> value="Salvar e Voltar" onclick="salvaLicitacao();"/>
			<input type="button" name="botao" value="Voltar" onclick="window.location='?modulo=principal/exibeLicitacao&acao=A';"/>
		<?php
		else:
		?>
			<input type="button" name="botao" <?php echo $desabilita; ?> value="Salvar" onclick="salvaLicitacao();"/>
                        <input type="button" name="botao" value="Voltar" id="voltar" onclick="window.location='?modulo=principal/listaLicitacao&acao=A';"/>
			<?php 
			if ( $_GET['acao'] == 'E' ):
			?>
			<input type="button" name="botao" value="Nova Licitação" onclick="window.location='?modulo=principal/cadLicitacao&acao=A';"/>
			<?php 
			endif;
			?>
		<?php 
		endif;
		?>	
		</td>
	</tr>
</table>
</form>
<script type="text/javascript">
DownloadArquivo = function(arqid){
	window.location = '?modulo=principal/cadLicitacao&acao=E&requisicao=download&arqid='+arqid;
}

function adicionarFases( objeto ){
	return windowOpen( '?modulo=principal/vincFaseLicitacao&acao=A&objeto='+objeto,'blank','height=450,width=650,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}

function abreNumLicUasg( id ){
	
	var tr_uasg    = document.getElementById( 'tr_uasg' );
	var tr_numlic  = document.getElementById( 'tr_numlic' );

	if( id == 1 || id == 2 || id == 3 || id == 4 ){
		tr_numlic.style.display = (document.selection ? 'block' : 'table-row');
	}else{
		tr_numlic.style.display = 'none';
	}
}

abreNumLicUasg(<?php echo $molid ?>);

function salvaLicitacao(){

	var obData = new Data();

	var tflid = document.getElementsByName("tflid[]");
	var molid = document.getElementById( "molid" ).value;
	
	var flcdata = document.getElementsByName('flcdata[]');
	
	if( (tflid.length == 0) /*&& (superuser == 1)*/){
		alert( "Favor inserir ao menos uma das Fases de Licitação!" );
		return false;
	}else{
		var flag = false;
	
		for( i = 0; i < tflid.length; i++ ){
			
			if( Number(tflid[i].value) == 2 || Number(tflid[i].value) == 9 ){
				flag = true;
				break;
			}
		}
	}

	if( !flag /*&& (superuser == 1)*/){
		alert("Favor inserir a fase Publicação do Edital ou Homologação da Licitação!");
		return false;
	}

	if ( molid == '' /*&& (superuser == 1)*/){
		alert( "O campo Modalidade de Licitação deve ser preenchido!" );
		return false;
	}else{
		if ( document.getElementById('obrid').tagName == 'SELECT' ){
			selectAllOptions(document.getElementById('obrid'));
		}	
		document.getElementById( "formulario" ).submit();
	}
}

function atualizaFase(flcid){
	return windowOpen( '?modulo=principal/vincFaseLicitacao&acao=A&flcid='+flcid,'blank','height=450,width=650,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}

function RemoveLinha(index,id){
	if( confirm('Tem certeza que deseja excluir este registro?') ){
		table = window.document.getElementById("faseslicitacao");
//		var conteiner = window.document.getElementById("conteinerAcao");
//		conteiner.innerHTML += "<input type='hidden' name='acaoFases[]' id='acaoFases"+id+"' value='"+id+"'/>";
		table.deleteRow(index);
		
		$.ajax({
				  url  		 : '?modulo=principal/cadLicitacao&acao=A',
				  type 		 : 'post',
				  data 		 : {requisicao : 'excluirfase', 
				  		  	    flcid 	   : id},
				  dataType   : "html",
				  async		 : false,
				  beforeSend : function (){
				  	divCarregando();
				  },
				  error 	 : function (){
				  	divCarregado();
				  },
				  success	 : function ( data ){
				  	alert('Fase excluída com sucesso.');
				  	divCarregado();
				  }
				});		
	}
}	

</script>
<?php
        $objObras = new Obras($obrid);
        $blockEdicao = $objObras->verificaObraVinculada();
        if($blockEdicao){
            echo '<script type="text/javascript">';
            echo " setTimeout(bloqueiaForm('formulario'), 500);
                   function bloqueiaForm(idForm){
                      jQuery('#'+idForm).find('input, textarea, button, select').attr('disabled','disabled');
                      jQuery('#'+idForm).find('a, span').attr('onclick','alert(\"Você não pode editar os dados da Obra Vinculada.\")');
                      jQuery('#voltar').attr('disabled', false);
                   }
                 ";
            echo '</script>';
        }
?>
