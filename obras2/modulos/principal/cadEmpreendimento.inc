<?php 

function removeAnexo(){
	global $db;
	
	$empreendimento = new Empreendimento();
	$empreendimento->excluirImagem( $_POST['empid'], $_POST['arqid'] );
	$db->commit();
	echo "true";
		
//	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
//	$file = new FilesSimec('arqsituacaodominial','arqid','obras');
//	if($file->setRemoveUpload($_REQUEST['arqid'])){
//		echo "true";
//	}
	die;
}

function salvar(){
	
	global $db;

	$empreendimento = new Empreendimento( $_REQUEST['empid'] );
	
    $arquivo = $_FILES["UploadPhoto"];
    $arqid   = "";
    
    if ( $_FILES["UploadPhoto"] && $arquivo["name"] && $arquivo["type"] && $arquivo["size"] ){
        if($_REQUEST['empid'] && $_REQUEST['arqid'])
            $empreendimento->excluirImagem($_REQUEST['empid'], $_REQUEST['arqid']);

		include_once APPRAIZ."includes/classes/fileSimec.class.inc";
            
        $file = new FilesSimec();
        $file->setPasta('obras2');
        $file->setUpload(null,null,false);
        $arqid = $file->getIdArquivo();
    }
	
	$endereco = new Endereco( $empreendimento->endid );
	$dadosEnd 			 		= $_POST['endereco'];
	// Foi colocado o TPEID do objeto pq o objeto utiliza o mesmo endereço da obra.
	$dadosEnd['tpeid'] 			= TIPO_ENDERECO_OBJETO;
	$dadosEnd['endcep'] 		= str_replace(Array('.','-'),'',$_POST['endereco']['endcep']);
	$dadosEnd['medlatitude']	= $_POST['graulatitude'].".".$_POST['minlatitude'].".".$_POST['seglatitude'].".".$_POST['pololatitude'];
	$dadosEnd['medlongitude']	= $_POST['graulongitude'].".".$_POST['minlongitude'].".".$_POST['seglongitude'];
	$endid 						= $endereco->popularDadosObjeto( $dadosEnd )
		   								   ->salvar();
		   								   
	$arNulo = array();
	if( empty($_REQUEST['demid']) ){
		$arNulo[] = 'demid';
	}		   								   
	
	$dados 			 			= $_POST;
	$dados['empvalorprevisto']	= str_replace(Array('.',','),Array('','.'),$dados['empvalorprevisto']);
	$dados['usucpf'] 			= $_SESSION['usucpf'];
	$dados['entidunidade'] 		= $_REQUEST['entid'];
	$dados['entidcampus'] 		= ($_REQUEST['entidcampus'] ? $_REQUEST['entidcampus'] : null);
	$dados['demid'] 			= ($_REQUEST['demid'] ? $_REQUEST['demid'] : null);
	$dados['endid']				= $endid;
	$dados['prfid']				= ($dados['prfid'] ? $dados['prfid'] : null);
	$dados['arqid']				= ($dados['arqid'] ? $dados['arqid'] : null);
	$dados['arqid']				= ($arqid ? $arqid : $dados['arqid']);
	$empid 						= $empreendimento->popularDadosObjeto( $dados )
		   										 ->salvar(true, true, $arNulo);
	if ( empty($_REQUEST['empid']) ){

		$endereco->endid = null;
		$endid = $endereco->salvar();
		
		$dadosObra 			 		   = $dados;	   										 
		$dadosObra['empid']  		   = $empid;	   										 
		$dadosObra['endid']  		   = $endid;	   										 
		$dadosObra['entid'] 		   = $dados['entidunidade'];
		$dadosObra['obrnome'] 		   = $dados['empdsc'];
		$dadosObra['obrvalorprevisto'] = $dados['empvalorprevisto'];
		$dadosObra['obrdsc'] 		   = $dados['empcomposicao'];
		
		$obra = new Obras();
		$obra->popularDadosObjeto( $dadosObra )
			 ->salvar();
	}	   										 
	$db->commit();
	
	$_SESSION['obras2']['empid'] = $empid;
	die('<script>
			alert(\'Operação realizada com sucesso!\');
			location.href=\'?modulo=principal/cadEmpreendimento&acao=A&empid='.$empid.'\';
		 </script>');
}

function litaPrograma(){
	
	global $db;
	
	$programaFonte = new ProgramaFonte();
	$param = array('tpoid' => $_REQUEST['tpoid'],
				   'orgid' => $_SESSION['obras2']['orgid']);
	$sql = $programaFonte->listaCombo( $param );
	$db->monta_combo('prfid',$sql, 'S','Selecione...','','','',200,'N', 'prfid', '', '', '');
}

function comboPreObra(){
	global $db;
	
	$demanda = new Demanda();
	
	$arParam['orgid'] 		 = $_SESSION['obras2']['orgid'];
	$arParam['entid'] 		 = $_POST['entidunidade'];
	$arParam['entid_campus'] = $_POST['entidcampus'];
	
	$db->monta_combo('demid', $demanda->listaCombo( $arParam ), 'S','Selecione...','','','',200,'N', 'demid', '', $empreendimento->demid, '');
}

if( $_REQUEST['req'] ){
	$_REQUEST['req']();
	die();
}

// Inclusão de arquivos do componente de Entidade
require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

$_SESSION['obras2']['empid'] = $_REQUEST['empid'] ? $_REQUEST['empid'] : $_SESSION['obras2']['empid'];
$empid = $_SESSION['obras2']['empid'];
if( $empid ){
	$db->cria_aba(ID_ABA_EMP_CADASTRADO,$url,$parametros);
}else{
	$db->cria_aba(ID_ABA_CADASTRA_EMP,$url,$parametros);
}
monta_titulo( 'Cadastro de Empreendimento', '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.' );

$empreendimento = new Empreendimento( $_SESSION['obras2']['empid'] );
$orgid 			= $_SESSION['obras2']['empid'] ? $empreendimento->orgid : $_SESSION['obras2']['orgid'];
//$docid 			= pegaDocidEmpreendimento( $empreendimento->empid );

$orgao 				= new Orgao($orgid);
$tipologiaObra 		= new TipologiaObra();
$programaFonte 		= new ProgramaFonte();
$modalidadeEnsino 	= new ModalidadeEnsino();
$tipoObra 			= new TipoObra();
$classificacaoObra 	= new ClassificacaoObra();
$endereco 			= new Endereco( $empreendimento->endid );

$orgid = $_SESSION['obras2']['orgid'];

if ( possui_perfil( PFLCOD_SUPER_USUARIO ) ){
	$habilitado = true;
	$habilita = 'S';
}else{
	$habilitado = false;
	$habilita = 'N';
}

// For uma obra do FNDE não possui campus
if ($orgid == ORGID_EDUCACAO_BASICA || $orgid == ORGID_ADMINISTRATIVO || $orgid == ORGID_HOSPITAIS) {
	$tr_campus = 'display:none;';
}
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>jQuery.noConflict();</script>
<script src="/includes/prototype.js"></script>
<script src="/includes/entidades.js"></script>
<script src="/includes/calendario.js"></script>
<script type="text/javascript">
function carregaPreObra( entidunidade, entidcampus ){
	entidunidade = entidunidade || '';
	entidcampus = entidcampus || '';

	
	jQuery('#entidcampus').unbind('change');
	jQuery('#entidcampus').change(function (){
		carregaPreObra( null, this.value );
	});

	jQuery.ajax({
		type: "POST",
		url: window.location.href,
		data: "req=comboPreObra&entidunidade=" + entidunidade + '&entidcampus=' + entidcampus,
		async: false,
		success: function(msg){
			jQuery('#tdPreObra').html(msg);
		}
	});

}

function abreEdicaoEndereco(){
	windowOpen( '?modulo=principal/editaEndereco&acao=E','blank','height=500,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}

function inserirEntidade(entid, orgid){
	if (entid){
		return windowOpen( '?modulo=principal/inserir_entidade&acao=A&busca=entnumcpfcnpj&entid=' + entid + '&orgid=' + orgid,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
	}else{
		return windowOpen( '?modulo=principal/inserir_entidade&acao=A&orgid=' + orgid,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
	}
}

function atualizaPrograma( tpoid ){

	jQuery.ajax({
		type: "POST",
		url: window.location.href,
		data: "req=litaPrograma&tpoid="+tpoid,
		async: false,
		success: function(msg){
			jQuery('#td_programa').html(msg);
		}
	});
}

jQuery(document).ready(function(){
	jQuery('[type="text"]').keyup();
	jQuery('.salvar').click(function(){
		var mensagem = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
		var stop 	 = false;
		
		if (jQuery('#entid').val() == ""){
			mensagem += 'Unidade Implantadora \n';
			stop = true;
		}
		
		if (jQuery('#empesfera').val() == ""){
			mensagem += 'Esfera \n';
			stop = true;
		}
		
		if (jQuery('#empdsc').val() == ""){
			mensagem += 'Nome do Empreendimento \n';
			stop = true;
		}
		
		if (jQuery('#tpoid').val() == ""){
			mensagem += 'Tipologia do Empreendimento \n';
			stop = true;
		}
		
		if (jQuery('#tobid').val() == ""){
			mensagem += 'Tipo do Empreendimento \n';
			stop = true;
		}
		
		if (jQuery('#cloid').val() == ""){
			mensagem += 'Classificação do Empreendimento \n';
			stop = true;
		}
		
		if (jQuery('#empcomposicao').val() == ""){
			mensagem += 'Descrição / Composição do Empreendimento \n';
			stop = true;
		}
		
		if (jQuery('#empvalorprevisto').val() == ""){
			mensagem += 'Valor Previsto (R$) \n';
			stop = true;
		}
		
		if (jQuery('#endcep').val() == ""){
			mensagem += 'CEP \n';
			stop = true;
		}
//		jQuery('.obrigatorio').each(function(){
//			if( jQuery(this).val() == '' ){
//				stop = true;
//				alert('Campo obrigatório.');
//				jQuery(this).focus();
//				return false;
//			}
//		});
		if( stop ){
			alert( mensagem );
			return false;
		}
		jQuery('#formEmpreendimento').submit();
	});
	

	jQuery('#imgExcluir').click(function(){
		//obj = jQuery(this);
		if(confirm('Deseja excluir a foto?')){
			jQuery.ajax({
				url     : '?modulo=principal/cadEmpreendimento&acao=A',
				type    : 'post',
				data    : 'req=removeAnexo&empid=' + jQuery('#empid').val() + '&arqid=' + jQuery('#arqid').val(),
				success : function(e){									
					if( e=='true' ){
						jQuery('#div_imagem').remove();
						jQuery('#arqid').val('');
					}else{
						alert('Não foi possível deletar a foto!');
					}
				}
			});
										
		}
	});
	
	
});

</script>
<form method="post" name="formEmpreendimento" id="formEmpreendimento" enctype="multipart/form-data">
	<input type="hidden" name="req" value="salvar"/>
	<input type="hidden" name="empid" id="empid" value="<?=$empreendimento->empid ?>"/>
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" width="20%">Tipo de Ensino:</td>
			<td>
				<input type="hidden" name="orgid" value="<?=$_SESSION['obras2']['orgid'] ?>"/>
				<?=$orgao->orgdesc ?>
			</td>
			<td rowspan="10" align="right" valign="top" width="1">
            <?php
            // Barra de estado WORKFLOW
//                wf_desenhaBarraNavegacao($docid, array('empid' =>  $empid));
            ?>
            </td>
		</tr>
		
        <tr>
            <td class="SubTituloDireita" width="30%" valign="top">
                Foto:
            </td>	
            <td>
                <?php $display = ($empreendimento->arqid ? 'display:block;' : 'display:none;');?>
                <div id="div_imagem" style="border:2px dashed #CCCCCC; padding:2px; margin-bottom:5px; width:350px; height:200px;<?=$display ?>">
                    <?php //if($permissao_formulario == 'S'): ?>
                        <img id="imgExcluir" src="../imagens/excluir_2.gif" style="position:absolute;cursor:pointer" title="Excluir Foto Atual"/>
                    <?php //endif; ?>
                    <img id="PhotoPrev" src="../slideshow/slideshow/verimagem.php?&arqid=<?=$empreendimento->arqid;?>&newwidth=350&newheight=200" style="width:350px;height:200px;"/>
                </div>
                <input type="hidden" name="arqid" id="arqid" value="<?=$empreendimento->arqid;?>" />
                <?php //if($permissao_formulario == 'S'): ?>
                    <input type="file" name="UploadPhoto" id="UploadPhoto" size="55"/>
                <?php //endif; ?>
            </td>
        </tr>
		
		<tr>
			<td class="SubTituloDireita">Unidade Implantadora:</td>
			<td>
				<?php 
				
				$entid_permitidas = obras_pegarUnidadesPermitidas();
				
				if ( !$_SESSION['obras2']["empid"] ){
					
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
						$db->monta_combo("entidunidade", $sql, 'S', "Selecione...", '', '', '', '340', 'S', 'entidunidade');
		
					}else{
							
						$entidade = new Entidade($empreendimento->entidunidade);
						$entnome  = $entidade->entnome;
						$entid    = $entidade->getPrimaryKey();
						?> 
						<input type="hidden" name="entid" id="entid" value="<? if(isset($_SESSION['obras2']["empid"])) echo $entid; ?>" class="obrigatorio"> 
						<?
					}
		
				}else{
		
					$entidade = new Entidade($empreendimento->entidunidade);
					$entnome = $entidade->entnome;
					$entid   = $entidade->getPrimaryKey();
				?> 
					<input type="hidden" name="entid" id="entid" value="<? if(isset($_SESSION['obras2']["empid"])) echo $entid; ?>" class="obrigatorio"> 
				<?
				}
				?> 
				<span id="entnome">
					<?php echo $entnome; ?> 
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
			<td id="mostracampus">
			<?php
			$campus 	 = new Entidade($empreendimento->entidcampus);
			$campusnome  = $campus->entnome;
			$entidcampus = $campus->getPrimaryKey();
	
			if ($_SESSION['obras2']["empid"] && $entid){
				switch ($orgid){
					case 1:
						$orgaoSesu  = 'AND ef.funid = 18';
						$orgaoSesu2 = 'AND ef.funid = 75';
						break;
					case 2:
						$orgaoSesu  = 'AND ef.funid = 17';
						$orgaoSesu2 = 'AND ef.funid = 75';
						break;
					case ORGID_HOSPITAIS:
						$orgaoSesu  = 'AND ef.funid = ' . ID_HOSPITAL;
						$orgaoSesu2 = 'AND ef.funid = ' . ID_HOSPITAL;
						break;
				}
	
				$sql = "SELECT
							et.entid as codigo,
							entnome as descricao
						FROM
							entidade.entidade et
						INNER JOIN
							entidade.funcaoentidade ef ON et.entid = ef.entid
						INNER JOIN
							entidade.funentassoc ea ON ea.fueid = ef.fueid 
						WHERE
							ea.entid = {$entid} " . $orgaoSesu .  " OR
							ea.entid = {$entid} " . $orgaoSesu2 . " AND
							et.entstatus = 'A'";
				$db->monta_combo("entidcampus", $sql, $habilita, "Selecione...", 'carregaPreObra( null, this.value );', '', '', '340', 'N', 'entidcampus');
			}
			?>
			</td>
		</tr>
		<?php
		if ( ($_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_SUPERIOR || $_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_PROFISSIONAL) ):
		?>		
		<tr>
			<td class="SubTituloDireita">Pré-obra:</td>
			<td id="tdPreObra">
				<?php 
				$demanda = new Demanda();
				
				$arParam['orgid'] 		 = $_SESSION['obras2']['orgid'];
				$arParam['entid'] 		 = $empreendimento->entidunidade;
				$arParam['entid_campus'] = $empreendimento->entidcampus;
				
				$db->monta_combo('demid', $demanda->listaCombo( $arParam ), $habilita,'Selecione...','','','',200,'N', 'demid', '', $empreendimento->demid, '');
				?>
			</td>
		</tr>
		<?php 
		endif;
		?>
		<tr>
			<td class="SubTituloDireita">Esfera:</td>
			<td>
				<?php 
				$sql = Array(Array('codigo'=>'M', 'descricao'=>'Municipal'),
							 Array('codigo'=>'E', 'descricao'=>'Estadual'),
							 Array('codigo'=>'F', 'descricao'=>'Federal'));
				$db->monta_combo('empesfera',$sql, $habilita,'Selecione...','','','',200,'S', 'empesfera', '', $empreendimento->empesfera, '');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Nome do Empreendimento:</td>
			<td><?=campo_texto('empdsc','S',$habilita,'',70,100,'','', '', '', '', 'id="empdsc"', '', $empreendimento->empdsc);?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Tipologia do Empreendimento:</td>
			<td>
				<?php 
				$sql = $tipologiaObra->listaCombo( array("orgid" => $_SESSION['obras2']['orgid']) );
				$db->monta_combo('tpoid',$sql, $habilita,'Selecione...','atualizaPrograma','','',200,'S', 'tpoid', '', $empreendimento->tpoid, '');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Programa:</td>
			<td id="td_programa">
				<?php 
				$param = array('tpoid' => $empreendimento->tpoid,
				   			   'orgid' => $_SESSION['obras2']['orgid']);
				$sql = $programaFonte->listaCombo( $param );
				$db->monta_combo('prfid',$sql, $habilita,'Selecione...','','','',200,'N', 'prfid', '', $empreendimento->prfid, '');
				?>
			</td>
		</tr>
		<?php
		if ( $_SESSION['obras2']['orgid'] != ORGID_EDUCACAO_SUPERIOR ):
		?>
		<tr>
			<td class="SubTituloDireita">Modalidade de Ensino:</td>
			<td>
				<?php 
				$sql = $modalidadeEnsino->listaCombo();
				$db->monta_combo('moeid',$sql, $habilita,'Selecione...','','','',200,'S', 'moeid', '', $empreendimento->moeid, '');
				?>
			</td>
		</tr>
		<?php
		endif;
		?>
		<tr>
			<td class="SubTituloDireita">Tipo do Empreendimento:</td>
			<td>
				<?php 
				$sql = $tipoObra->listaCombo();
				$db->monta_combo('tobid',$sql, $habilita,'Selecione...','','','',200,'S', 'tobid', '', $empreendimento->tobid, '');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Classificação do Empreendimento:</td>
			<td>
				<?php 
				$sql = $classificacaoObra->listaCombo();
				$db->monta_combo('cloid',$sql, $habilita,'Selecione...','','','',200,'S', 'cloid', '', $empreendimento->cloid, '');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Descrição / Composição do Empreendimento:</td>
			<td>
				<?=campo_textarea('empcomposicao', 'S', $habilita, '', 150, 5, '', '', '', '', '', '', $empreendimento->empcomposicao, $param) ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Valor Previsto (R$):</td>
			<td colspan="2"><?=campo_texto('empvalorprevisto','S',$habilita,'',20,20,'[.###],##','', '', '', '', 'id="empvalorprevisto"', '', $empreendimento->empvalorprevisto);?></td>
		</tr>
<?php
		
		$habilitadoEnd = $habilitado;
		$habilitaEnd   = $habilita;
		$classEnd      = 'CampoEstilo';
		if ( $endereco->endid || $habilitado == false ){
			$habilitadoEnd = false;
			$habilitaEnd   = 'N';
			$classEnd      = 'disabled';
		}

?>		
		<tr>
			<td colspan="3">Local do Empreendimento</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>CEP:</label></td>
			<td colspan="2">
				<input type="text" name="endereco[endcep]" 
					   onkeyup="this.value=mascaraglobal('##.###-###', this.value);" 
					   onblur="Entidade.__getEnderecoPeloCEP(this);" id="endcep" 
					   value="<?=$endereco->endcep ?>" size="13" maxlength="10" 
					   <?php echo ($habilitadoEnd ? 'class="CampoEstilo"' : 'class="disabled" readonly="readonly"') ?>/>
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
			</td>
		</tr>
		<tr id="escolha_logradouro_id" style="display:none">
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Sugestão de Logradouro:</label></td>
			<td colspan="2">
				<input type="text" name="endlog" 
					   id="endlog" value="<?=$endereco->endlog ?>" size="48" 
					   <?php echo ($habilitadoEnd ? 'class="CampoEstilo"' : 'class="disabled" readonly="readonly"') ?>/>
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
            	<label id="lbLogadouro"> Logradouro: </label>
            </td>
            <td colspan="2">
           		<input type="text" name="endereco[endlog]" 
					   id="endlogradouro" value="<?=$endereco->endlog ?>" size="65" 
					   <?php echo ($habilitadoEnd ? 'class="CampoEstilo"' : 'class="disabled" readonly="readonly"') ?>/>
      		</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Número:</label></td>
      		<td colspan="2">
  				<input type="text" name="endereco[endnum]" 
					   id="endnum" value="<?=$endereco->endnum ?>" size="13" maxlength="8" 
					   <?php echo ($habilitadoEnd ? 'class="CampoEstilo"' : 'class="disabled" readonly="readonly"') ?>/>
            </td>
      	</tr>
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Complemento:</label></td>
      		<td colspan="2">
           		<input type="text" name="endereco[endcom]" 
					   id="endcom" value="<?=$endereco->endcom ?>" size="65" maxlength="100" 
					   <?php echo ($habilitadoEnd ? 'class="CampoEstilo"' : 'class="disabled" readonly="readonly"') ?>/>
          	</td>
    	</tr>
		<tr>
         	<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Bairro:</label></td>
     		<td colspan="2">
    			<input type="text" name="endereco[endbai]" 
					   id="endbai" value="<?=$endereco->endbai ?>" size="20" 
					   <?php echo ($habilitadoEnd ? 'class="CampoEstilo"' : 'class="disabled" readonly="readonly"') ?>/>
        	</td>
    	</tr>
    	<tr>
          	<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Município/UF: </label></td>
          	<td colspan="2">
              	<input type="text" name="mundescricao"  
					   id="mundescricao" value="<?=$endereco->getMunDescricao()?>" size="20" 
					   <?php echo ($habilitadoEnd ? 'class="CampoEstilo"' : 'class="disabled" readonly="readonly"') ?>/>
              	<input type="hidden" name="endereco[muncod]" id="muncod" class="CampoEstilo" value="<?=$endereco->muncod ?>" />
             	<input type="text" name="endereco[estuf]" 
					   id="estuf" value="<?=$endereco->estuf ?>" style="width: 5ex; padding-left: 2px" 
					   <?php echo ($habilitadoEnd ? 'class="CampoEstilo"' : 'class="disabled" readonly="readonly"') ?>/>
         	</td>
      	</tr>
      	<tr>
			<td colspan="3">Coordenadas Geográficas</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Latitude</td>
			<td colspan="2"><?php 
			$medlatitude = $endereco->medlatitude;
			$latitude = explode(".", $medlatitude);
			$graulatitude = $latitude[0];
			$minlatitude = $latitude[1];
			$seglatitude = $latitude[2];
			$pololatitude = $latitude[3];
			
				
			?> <?= campo_texto( 'graulatitude', 'N', $habilitaEnd, '', 2, 2, '##', '', 'left', '', 0, 'id="graulatitude"'); ?>
			° <?= campo_texto( 'minlatitude', 'N', $habilitaEnd, '', 2, 2, '##', '', 'left', '', 0, 'id="minlatitude" '); ?>
			' <?= campo_texto( 'seglatitude', 'N', $habilitaEnd, '', 2, 2, '##', '', 'left', '', 0, 'id="seglatitude" '); ?>
			'' <input type="hidden" name="pololatitude" id="pololatitude" value="<?php echo trim($pololatitude) ?>">
			<?php 
			if (trim($pololatitude) == "S") {
				echo "&nbsp;<span id=pololatitude_>S</span>"; 
			} elseif(trim($pololatitude) == "N") {
				echo "&nbsp;<span id=pololatitude_>N</span>";
			} else {
				echo "&nbsp;<span id=pololatitude_></span>";
			}
			?>
			<?php print obrigatorio(); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Longitude</td>
			<td colspan="2"><?php 
			$medlongitude = $endereco->medlongitude;
			$longitude = explode(".", $medlongitude);
			$graulongitude = $longitude[0];
			$minlongitude = $longitude[1];
			$seglongitude = $longitude[2];
			?> <?= campo_texto( 'graulongitude', 'N', $habilitaEnd, '', 2, 2, '##', '', 'left', '', 0, 'id="graulongitude"'); ?>
			° <?= campo_texto( 'minlongitude', 'N', $habilitaEnd, '', 2, 2, '##', '', 'left', '', 0, 'id="minlongitude"'); ?>
			' <?= campo_texto( 'seglongitude', 'N', $habilitaEnd, '', 2, 2, '##', '', 'left', '', 0, 'id="seglongitude"');  ?>
			'' &nbsp W <?php print obrigatorio(); ?></td>
		</tr>
		<?php
		#if($graulongitude != "" && $minlongitude != "" && $seglongitude != "" && $graulatitude != "" && $minlatitude != "" && $seglatitude != ""){ ?>
		<tr>
			<td class="SubTituloDireita"></td>
			<td colspan="2">
<?php 
			if ( $habilitadoEnd ):
?>
				<a href="#" onclick="abreMapa();">Visualizar / Buscar No Mapa</a>
<?php 
			elseif (possui_perfil( array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC) )):
?>
				<a href="#" onclick="abreEdicaoEndereco();">Editar Endereço da Obra</a>
<?php 
			endif;
?>
				<input style="display: none;" type="text" name="endereco[endzoom]"
					   id="endzoom"
					   value=<?php echo ($endereco->endzoom ==null) ? "15" : $endereco->endzoom;?>>
			</td>
		</tr>
		
		
		
		<tr>
			<td colspan="3" align="center" bgcolor="#dcdcdc"><b>Contatos</b></td>
		</tr>
		<tr>
			<td colspan="3">
				<?php
				$contato 		= new Contato();
				$_POST['estuf'] = $endereco->estuf;
				$_POST['orgid'] = $orgid;
				$sql	 		= $contato->listaResumoSql( $_POST );
				
				$cabecalho = array("Nome", "Telefone");
				$db->monta_lista($sql,$cabecalho,30,5,'N','center','');
				?>
			</td>
		</tr>
		
		
		
		<tr>
			<td style="background-color:#DCDCDC" colspan="3" align="center">
				<input type="button" name="salvar" class="salvar" value="Salvar"  <?php echo ($habilitado ? '' : 'disabled="disabled"') ?>/>
			</td>
		</tr>
	</table>
</form>