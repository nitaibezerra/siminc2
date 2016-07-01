<?php 
function salvar(){
	
	global $db;

	$demanda = new Demanda( $_REQUEST['demid'] );
	
	$endereco 					= new Endereco( $demanda->endid );
	$dadosEnd 			 		= $_POST['endereco'];
	$dadosEnd['endcep'] 		= str_replace(Array('.','-'),'',$_POST['endereco']['endcep']);
	$dadosEnd['medlatitude']	= $_POST['graulatitude'].".".$_POST['minlatitude'].".".$_POST['seglatitude'].".S";
	$dadosEnd['medlongitude']	= $_POST['graulongitude'].".".$_POST['minlongitude'].".".$_POST['seglongitude'].".W";
	$endid 						= $endereco->popularDadosObjeto( $dadosEnd )
										   ->salvar();
	
	$dados 			 			= $_POST['demanda'];
	if( $demanda->docid == ''  ){
		
		require_once APPRAIZ . 'includes/workflow.php';
		
		$dados['docid']			= wf_cadastrarDocumento( TPDID_DEMANDA, 'Demanda de Obra' );
	}
	$dados['endid']				= $endid;
	$dados['demvalortotal_prev']= str_replace(Array('.',','),Array('','.'),$dados['demvalortotal_prev']);
	$dados['demano']			= date('Y');
	$dados['demqtdalunos']		= $dados['demqtdalunos'] != '' ? $dados['demqtdalunos'] : 0;
	$demid 						= $demanda->popularDadosObjeto( $dados )
		   		  					   	  ->salvar();
	
	$db->commit();
	
	$_SESSION['obras2']['demid'] = $demid;
	die('<script>
			alert(\'Operação realizada com sucesso!\');
			location.href=\'?modulo=principal/preobra/cadDemanda&acao=A&demid='.$demid.'\';
		 </script>');
}


if( $_REQUEST['req'] ){
	$_REQUEST['req']();
	die();
}

// Inclusï¿½o de arquivos do componente de Entidade
require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
$_SESSION['obras2']['demid'] = $_REQUEST['demid'] ? $_REQUEST['demid'] : $_SESSION['obras2']['demid'];
$arAba = getArAba('caddemandas');
echo montarAbasArray($arAba, "?modulo=principal/preobra/cadDemanda&acao=A&demid=" . $_SESSION['obras2']['demid']);

$demanda					= new Demanda( $_SESSION['obras2']['demid'] );
$orgid 						= $_SESSION['obras2']['orgid'];
$orgao 						= new Orgao($orgid);
$demandaClassificacaoObra 	= new DemandaClassificacaoObra();
$demandaTipoObra 			= new DemandaTipoObra();
$demandaSituacaoProjeto		= new DemandaSituacaoProjeto();
$endereco 					= new Endereco( $demanda->endid );

$orgid = $_SESSION['obras2']['orgid'];

monta_titulo( 'Demanda', '' );

$habilitado = true;
$habilita = 'S';

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>jQuery.noConflict();</script>
<script src="/includes/prototype.js"></script>
<script src="/includes/entidades.js"></script>
<script src="/includes/calendario.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript">
 
function inserirEntidade(entid, orgid){
	jQuery('[id^="rpuid_"]').each(function(){
		jQuery(this).remove();
	});
	return windowOpen( '?modulo=principal/inserir_entidade&acao=A&busca=entnumcpfcnpj&entid=' + entid + '&orgid=' + orgid,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}

function visualizarEntidade(entid, orgid){
	if( entid ){
		return windowOpen( '?modulo=principal/inserir_entidade&acao=A&busca=entnumcpfcnpj&bloq=disabled="disabled"&entid=' + entid + '&orgid=' + orgid,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
	}
}

jQuery(document).ready(function(){
	jQuery('[type="text"]').keyup();
	jQuery('.salvar').click(function(){
		var stop = false;
		jQuery('.obrigatorio').each(function(){
			if( jQuery(this).val() == '' ){
				stop = true;
				alert('Campo obrigatório.');
				jQuery(this).focus();
				return false;
			}
		});
		if( stop ){
			return false;
		}
		if( jQuery('#entid').val() == '' ){
			alert('Escolha uma unidade implantadora.');
			jQuery('#entid').focus();
			return false;
		}
		jQuery('#formDemanda').submit();
	});
});
</script>
<form method="post" name="formDemanda" id="formDemanda">
	<input type="hidden" name="req" value="salvar"/>
	<input type="hidden" name="demanda[demid]" value="<?=$demanda->demid ?>"/>
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" width="20%">Tipo de Ensino:</td>
			<td>
				<input type="hidden" name="demanda[orgid]" value="<?=$_SESSION['obras2']['orgid'] ?>"/>
				<?=$orgao->orgdesc ?>
			</td>
			<?php if( $demanda->docid ){?>
			<td rowspan="6" width="15%">
				<center>
					<?php require_once APPRAIZ . 'includes/workflow.php';?>
					<?=wf_desenhaBarraNavegacao( $demanda->docid , array( ),  array('historico'=>true)); ?>
				</center>
			</td>
			<?php }?>
		</tr>
		<tr>
			<td class="SubTituloDireita">Unidade Implantadora:</td>
			<td>
				<?php 
				
				$entid_permitidas = obras_pegarUnidadesPermitidas();
				
				if ( !$_SESSION['obras2']["demid"] ){
					
					if ( $entid_permitidas ){
							
						$sql = "SELECT
									e.entid as codigo,
									e.entnome as descricao
								FROM
									entidade.entidade e
								INNER JOIN entidade.funcaoentidade fue ON fue.entid = e.entid
								INNER JOIN obras2.orgaofuncao orf ON orf.funid = fue.funid
								WHERE
									e.entid in ( ". implode( ", ", $entid_permitidas ) ." ) AND orf.orgid = ".$_REQUEST["org"];
						$db->monta_combo("demanda[entid]", $sql, 'S', "Selecione...", '', '', '', '340', 'S', 'entid');
		
					}else{
						
						$empreendimento = new Empreendimento( $_SESSION['obras2']['empid'] );
						$empreendimento->entidunidade;
							
						$entidade = new Entidade( $empreendimento->entidunidade );
						$entnome  = $entidade->entnome;
						$entid    = $entidade->getPrimaryKey();
						?> 
						<input type="hidden" name="demanda[entid]" id="entid" value="<? if(isset($_SESSION['obras2']["demid"])) echo $entid; ?>"> 
						<?
					}
		
				}else{
		
					$entidade = new Entidade($demanda->entid);
					$entnome = $entidade->entnome;
					$entid   = $entidade->getPrimaryKey();
				?> 
					<input type="hidden" name="demanda[entid]" id="entid" value="<? if(isset($_SESSION['obras2']["demid"])) echo $entid; ?>"> 
				<?
				}
				?> 
				<span id="entnome">
					<a onclick="visualizarEntidade(document.getElementById('entid').value,<?=$orgid ?>);" style="cursor:pointer">
						<?php echo $entnome; ?> 
					</a>
					<input type="hidden" name="entnome_h" id="entnome_h" value="<?php echo $entnome; ?>">
				</span>
				<?php 
				if( $habilitado && !$entid_permitidas ){ 
				?> 
				<input type="button" name="pesquisar_entidade" value="Pesquisar" style="cursor: pointer;"
				       onclick="inserirEntidade(document.getElementById('entid').value,<?=$orgid ?>);" <?php if($somenteLeitura=="N") echo "disabled"; ?>> 
				<img src="../imagens/obrig.gif" title="Indica campo obrigatório." border="0"> 
				<?php 
				} 
				?>
			</td>
		</tr>
		<tr id="campus" style="<?php echo $tr_campus; ?>">
			<td class="SubTituloDireita">Campus / Reitoria:</td>
			<td id="mostracampus"><?php
			$campus = new Entidade($demanda->entid_campus);
			$campusnome = $campus->entnome;
			$entidcampus = $campus->getPrimaryKey();
			
			if ($_SESSION["obras2"]["demid"] && $entid){
				
				switch($orgid) {
					case ORGID_HOSPITAIS:
						$sql = "SELECT ef.funid FROM entidade.entidade e
								INNER JOIN entidade.funcaoentidade ef ON e.entid = ef.entid
								WHERE e.entid = '$entid' AND ef.funid='".ID_HOSPITAL."'";
						$funid = $db->pegaUm($sql);
						break;
					case ORGID_EDUCACAO_SUPERIOR:
							$sql = "SELECT funid FROM entidade.entidade e
							INNER JOIN entidade.funcaoentidade ef ON e.entid = ef.entid
							WHERE	e.entid = '$entid' AND ef.funid = '".ID_UNIVERSIDADE."'";
						$funid = $db->pegaUm($sql);
						break;
					case ORGID_EDUCACAO_PROFISSIONAL:
						$sql = "SELECT funid FROM entidade.entidade e
								INNER JOIN entidade.funcaoentidade ef ON e.entid = ef.entid
								WHERE
									e.entid = '$entid' AND 
									ef.funid IN('".ID_ESCOLAS_TECNICAS."','".ID_ESCOLAS_AGROTECNICAS."','".ID_UNED."','".ID_SUPERVISIONADA."','".ID_REITORIA."')";
						$funid = $db->pegaUm($sql);
						break;
				}
				
				if( $funid ){
					switch($funid) {
						case ID_UNIVERSIDADE:
							$orgaoSesu  = 'AND ef.funid = '.ID_CAMPUS;
							$orgaoSesu2 = 'AND ef.funid = '.ID_REITORIA;
							break;
						case ID_ESCOLAS_TECNICAS:
						case ID_ESCOLAS_AGROTECNICAS:
							$orgaoSesu  = 'AND ef.funid = '.ID_UNED;
							$orgaoSesu2 = 'AND ef.funid = '.ID_REITORIA;
							break;
						case ID_HOSPITAL:
							$orgaoSesu  = 'AND ef.funid = '.ID_HOSPITAL;
							$orgaoSesu2 = 'AND ef.funid = '.ID_HOSPITAL;
							break;
					}
		
					$sql = "SELECT
								et.entid as codigo,
								entnome as descricao
							FROM
								entidade.entidade et
							INNER JOIN entidade.funcaoentidade ef ON et.entid = ef.entid
							INNER JOIN entidade.funentassoc ea ON ea.fueid = ef.fueid 
							WHERE
								ea.entid = {$entid} " . $orgaoSesu .  " OR
								ea.entid = {$entid} " . $orgaoSesu2 . " AND
								et.entstatus = 'A' ";
		
					$db->monta_combo("demanda[entid_campus]", $sql, $habilita, "Selecione...", '', '', '', '', 'S', 'entidcampus','',$entidcampus);
				}
			}
	
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Nome da Demanda:</td>
			<td><?=campo_texto('demanda[demnome]','S',$habilita,'',70,100,'','', '', '', '', 'id="demnome"', '', $demanda->demnome);?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Classificação da Demanda:</td>
			<td>
				<?php 
				$sql = $demandaClassificacaoObra->listaCombo();
				$db->monta_combo('demanda[dclid]',$sql, $habilita,'Selecione...','','','',200,'S', 'dclid', '', $demanda->dclid, '');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Tipo da Demanda:</td>
			<td>
				<?php 
				$sql = $demandaTipoObra->listaCombo();
				$db->monta_combo('demanda[dtoid]',$sql, $habilita,'Selecione...','','','',200,'S', 'dtoid', '', $demanda->dtoid, '');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Situação do Projeto da Demanda:</td>
			<td>
				<?php 
				$sql = $demandaSituacaoProjeto->listaCombo();
				$db->monta_combo('demanda[dspid]',$sql, $habilita,'Selecione...','','','',200,'S', 'dspid', '', $demanda->dspid, '');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Descrição / Composição da Obra:</td>
			<td>
				<?=campo_textarea('demanda[demdesc]', 'S', $habilita, '', 120, 5, '', '', '', '', '', '', $demanda->demdesc, $param) ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Area:</td>
			<td><?=campo_texto('demanda[demarea]','S',$habilita,'',20,20,'[.###]','', '', '', '', 'id="demarea"', '', $demanda->demarea);?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Valor Previsto (R$):</td>
			<td>
				<?=campo_texto('demanda[demvalortotal_prev]','S',$habilita,'',20,20,'[.###],##','', '', '', '', 'id="demvalortotal_prev"', '', $demanda->demvalortotal_prev);?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Quantidade de Alunos:</td>
			<td><?=campo_texto('demanda[demqtdalunos]','N',$habilita,'',20,20,'[.###]','', '', '', '', 'id="demqtdalunos"', '', $demanda->demqtdalunos);?></td>
		</tr>
		<tr>
			<td colspan="2">Local da Obra</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>CEP:</label></td>
			<td>
				<input type="text" name="endereco[endcep]" 
					   onkeyup="this.value=mascaraglobal('##.###-###', this.value);" 
					   onblur="Entidade.__getEnderecoPeloCEP(this);" class="CampoEstilo" id="endcep" 
					   value="<?=$endereco->endcep ?>" size="13" maxlength="10" />
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
			</td>
		</tr>
		<tr id="escolha_logradouro_id" style="display:none">
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Sugestão de Logradouro:</label></td>
			<td>
				<input type="text" name="endlog" class="CampoEstilo" id="endlog" value="<?=$endereco->endlog ?>" size="48" />
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
            	<label id="lbLogadouro"> Logradouro: </label>
            </td>
            <td>
           		<input type="text" name="endereco[endlog]" class="CampoEstilo" id="endlogradouro" value="<?=$endereco->endlog ?>" size="65" />
      		</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Número:</label></td>
      		<td>
  				<input type="text" name="endereco[endnum]" class="CampoEstilo" id="endnum" value="<?=$endereco->endnum ?>" size="13" maxlength="8" />
            </td>
      	</tr>
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Complemento:</label></td>
      		<td>
           		<input type="text" name="endereco[endcom]" class="CampoEstilo" id="endcom" value="<?=$endereco->endcom ?>" size="65" maxlength="100" />
          	</td>
    	</tr>
		<tr>
         	<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Bairro:</label></td>
     		<td>
    			<input type="text" name="endereco[endbai]" class="CampoEstilo" id="endbai" value="<?=$endereco->endbai ?>" size="20" />
        	</td>
    	</tr>
    	<tr>
          	<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Município/UF: </label></td>
          	<td>
              	<input type="text" name="mundescricao"  class="CampoEstilo" id="mundescricao" value="<?=$endereco->getMunDescricao()?>" size="20" />
              	<input type="hidden" name="endereco[muncod]" id="muncod" class="CampoEstilo" value="<?=$endereco->muncod ?>" />
             	<input type="text" name="endereco[estuf]" class="CampoEstilo" id="estuf" value="<?=$endereco->estuf ?>" style="width: 5ex; padding-left: 2px" />
         	</td>
      	</tr>
      	<tr>
			<td>Coordenadas Geográficas</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Latitude</td>
			<td>
				<?php 
				$medlatitude = $endereco->medlatitude;
				$latitude = explode(".", $medlatitude);
				$graulatitude = $latitude[0];
				$minlatitude = $latitude[1];
				$seglatitude = $latitude[2];
				$pololatitude = $latitude[3];
				?> 
				<?= campo_texto( 'graulatitude', 'N', $habilita, '', 2, 2, '##', '', 'left', '', 0, 'id="graulatitude"'); ?> °
				<?= campo_texto( 'minlatitude', 'N', $habilita, '', 2, 2, '##', '', 'left', '', 0, 'id="minlatitude" '); ?> ' 
				<?= campo_texto( 'seglatitude', 'N', $habilita, '', 2, 2, '##', '', 'left', '', 0, 'id="seglatitude" '); ?> '' 
				<?php 
				if (trim($pololatitude) == "S") {
					echo "&nbsp;<span id=pololatitude_>S</span>"; 
				}elseif(trim($pololatitude) == "N") {
					echo "&nbsp;<span id=pololatitude_>N</span>";
				}else {
					echo "&nbsp;<span id=pololatitude_></span>";
				}
				?>
				<input type="hidden" name="pololatitude" id="pololatitude" value="<?=trim($pololatitude) ?>">
				<?php print obrigatorio(); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Longitude</td>
			<td>
				<?php 
				$medlongitude = $endereco->medlongitude;
				$longitude = explode(".", $medlongitude);
				$graulongitude = $longitude[0];
				$minlongitude = $longitude[1];
				$seglongitude = $longitude[2];
				$pololongitude = $longitude[3];
				?> 
				<?= campo_texto( 'graulongitude', 'N', $habilita, '', 2, 2, '##', '', 'left', '', 0, 'id="graulongitude"'); ?> °
				<?= campo_texto( 'minlongitude', 'N', $habilita, '', 2, 2, '##', '', 'left', '', 0, 'id="minlongitude"'); ?> ' 
				<?= campo_texto( 'seglongitude', 'N', $habilita, '', 2, 2, '##', '', 'left', '', 0, 'id="seglongitude"');  ?> ''
				<?php 
				if (trim($pololongitude) == "W") {
					echo "&nbsp;<span id=pololatitude_>W</span>"; 
				}elseif(trim($pololongitude) == "E") {
					echo "&nbsp;<span id=pololatitude_>E</span>";
				}else {
					echo "&nbsp;<span id=pololatitude_></span>";
				}
				?>
				<input type="hidden" name="pololatitude" id="pololatitude" value="<?=trim($pololongitude) ?>">
				<?php print obrigatorio(); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"></td>
			<td><a href="#" onclick="abreMapa();">Visualizar / Buscar No Mapa</a>
			<input style="display: none;" type="text" name="endereco[endzoom]" id="endzoom"
				   value=<? if ( $endereco->endzoom == null ) echo "15"; else echo $endereco->endzoom;?>>
			</td>
		</tr>
		<tr>
			<td style="background-color:#DCDCDC" colspan="3" align="center">
				<input type="button" name="salvar" class="salvar" value="Salvar"/>
			</td>
		</tr>
	</table>
</form>