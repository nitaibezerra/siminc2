<?
 /*
   Sistema Simec
   Setor responsável: MEC
   Desenvolvedor: Equipe Desenvolvedores Simec
   Analista: Vitor Nunes Sad (vitor.sad@mec.gov.br)
   Programador: Fernando Rolim (fernandosilvabsb@hotmail.com)
   Módulo: salasremult
   Finalidade: permitir pesquisa de salas de recursos multifuncionais
    */

if(isset($_REQUEST['requisicao']) && $_REQUEST['requisicao'] == 'carregarEscolaPeloINEP') {
	$sql = "select 	ent.entid as entid, ent.entnome as entnome
			from  	entidade.entidade ent 
			where 	ent.entcodent = '" . $_REQUEST['entcodent'] . "' ";
	$escola = $db->pegaLinha($sql);
	$entid = $escola['entid'];
	$entnome = $escola['entnome'];
	echo("$entid,$entnome");
    die(); 
}

include_once( APPRAIZ . "includes/classes/fileSimec.class.inc");


if(isset($_REQUEST['requisicao']) && $_REQUEST['requisicao'] == 'download') {
	$file = new FilesSimec();
    $arquivo = $file->getDownloadArquivo($_REQUEST['arqid']);
    exit;
}

//requisicao=removeranexo&anxid=<?=$anexo['anxid']>&arqid=<=$anexo['arqid']>'
if(isset($_REQUEST['requisicao']) && $_REQUEST['requisicao'] == 'removeranexo') {
    $salid = $_REQUEST['salid'];
    $anxid = $_REQUEST['anxid'];
    $arqid = $_REQUEST['arqid'];
    removerAnexo($salid,$anxid,$arqid);
}

if(isset($_REQUEST['requisicao']) && $_REQUEST['requisicao'] == 'gravarTransferencia') 
{
    $salid = $_REQUEST['salid'];

    $entid 				 = $_REQUEST['entid'];
//    $saldataadesao 		 = $_REQUEST['saldataadesao'];
//    $saldataencerramento = $_REQUEST['saldataencerramento'];
    
    $entid_destino               = $_REQUEST['entid_destino'];
//    $saldataadesao_destino 		 = $_REQUEST['saldataadesao_destino'];
//    $saldataencerramento_destino = $_REQUEST['saldataencerramento_destino'];
	$tardatatransferencia = $_REQUEST['tardatatransferencia'];

//    gravarTransferencia( $salid ,
//    					 $entid , $saldataadesao , $saldataencerramento ,
//    					 $entid_destino , $saldataadesao_destino , $saldataencerramento_destino);

    gravarTransferencia( $salid , $entid , $entid_destino , $tardatatransferencia);
	
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo '<br>';


$salid = $_REQUEST['salid'];
$salanocriacao = $_REQUEST['salanocriacao_ano'];

if(isset($_REQUEST['requisicao']) && $_REQUEST['requisicao'] == 'inseriranexo') {
	$arqid = inserirArquivo();
	inserirAnexoSala($salid, $arqid);

} else if(isset($_REQUEST['requisicao']) && $_REQUEST['requisicao'] == 'gravarsala') {
	$salcodigosala 		 = $_REQUEST['salcodigosala'];
    $entid 		   		 = $_REQUEST['entid'];
    $esfid				 = $_REQUEST['esfid'];
    $tipid				 = $_REQUEST['tipid'];
    $sitid				 = $_REQUEST['sitid'];
    $salobservacao		 = $_REQUEST['salobservacao'];
	
	if( is_combo_set('salid') ) {
		$sql = "UPDATE salasrecmult.salas SET 
					  salcodigosala		  = '$salcodigosala' 
					, entid    			  = $entid
					, salanocriacao		  = $salanocriacao
					, esfid				  = $esfid 
					, tipid				  = $tipid 
					, sitid				  = $sitid 
					, salobservacao       = '$salobservacao' 
	    		WHERE salid = $salid;";
	} else {
		$nextval = (int)($_REQUEST['salcodigosala']);
		
		$sql = "INSERT INTO salasrecmult.salas (
					salid				, 
					salcodigosala		,
					entid    			,
					salanocriacao		,
					esfid				,
					tipid				,
					sitid				,
					salobservacao
	            )
	    		VALUES (
	    			   $nextval
	    			, '$salcodigosala/$salanocriacao'
	    			,  $entid 
	    			,  $salanocriacao
	    			,  $esfid 
	    			,  $tipid 
	    			,  $sitid 
	    			, '$salobservacao'
	    		);";

		$salid = $nextval;
	}
	
	//ver( $sql );
	
	$db->executar($sql);	  
	$db->commit();	
}

?>
	<script src="../../../includes/prototype.js"></script>
	<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
	<script type="text/javascript">
	function gravarTransferenciaExec() {
		if( isPreenchido('salid') && isPreenchido('entid_destino') && isPreenchido('tardatatransferencia') ) {
			$('requisicao').value = "gravarTransferencia";
			$('formulario').submit();
		} else {
			alert("Preencha todos os campos da Escola de Destino para gravar transferência.");
		}
	}


	function recuperarEntidadeEscola(entcodent) {
		if( entcodent=='' ) {
			return;
		}
	    var campoEntId = $('entid');
		var campoEntNome = $('entnome');
	    new Ajax.Request('salasrecmult.php?modulo=principal/cadsala&acao=A',
	    {
	        method: 'post',
	        parameters: '&requisicao=carregarEscolaPeloINEP&entcodent='+entcodent,
	        onComplete: function(res){
		    	var arrayResponse = res.responseText.split(",");
	            campoEntId.value = arrayResponse[0];
				campoEntNome.innerHTML = "<span>" + arrayResponse[1] + "</span>";	            
	        }
	    }); 
	}

	function recuperarEntidadeEscolaDestino(entcodentDestino) {
		if( entcodentDestino=='' ) {
			return;
		}
	    var campoEntId = $('entid_destino');
		var campoEntNome = $('entnome_destino');
	    new Ajax.Request('salasrecmult.php?modulo=principal/cadsala&acao=A',
	    {
	        method: 'post',
	        parameters: '&requisicao=carregarEscolaPeloINEP&entcodent='+entcodentDestino,
	        onComplete: function(res){
		    	var arrayResponse = res.responseText.split(",");
	            campoEntId.value = arrayResponse[0];
				campoEntNome.innerHTML = "<span>" + arrayResponse[1] + "</span>";	            
	        }
	    }); 
	}	

	function enviarArquivo() {
		if( verifiqueSeHaSalId() ) {
			$('formularioDocumento').submit();
		}
	}
	
	function verifiqueSeHaSalId() {
		if( $('salid') != "" ) {
			return true;
		} 
		return false; 
	}

	function excluir(url, msg) {
		if(confirm(msg)) {
			window.location=url;
		}
	}

	function exibirCamposTransferir() {
		$('td1').style.display="block";
		$('td2').style.display="none";
		$('transf_1').style.visibility="visible";
		$('transf_2').style.visibility="visible";
		$('transf_3').style.visibility="visible";
		$('transf_4').style.visibility="visible";
		$('transf_5').style.visibility="visible";
		$('gravarTransferencia').style.visibility="visible";
		$('exibirTransferir').style.visibility="collapse";
	}

	function isPreenchido(idCampo) {
		return $(idCampo) != "";
	}

	function gravarSala() {
		var okAnoCriacao = $('salanocriacao') != "";
		var okInep       = $('entcodent') != "";
		var okEsfera     = $('formulario').esfid.value != "";
		var okTipo       = $('formulario').tipid.value != ""; 

		if( okAnoCriacao && okInep && okEsfera && okTipo ) {
			document.formulario.submit();
			alert ('Dados gravados com sucesso.');
		} else {
			alert("Verifique os campos obrigatórios. Operação cancelada.");
		}		
	}

	</script>	

	<?php
		$db->cria_aba( $abacod_tela, $url, '' );
		$titulo_modulo = "Cadastro de Salas de Recursos Multifuncionais";
		monta_titulo( $titulo_modulo, '<div style="margin:3px;" align="right" class="' . $class . '"></div>' );
	?>
	
	<form action="?modulo=principal/cadsala&acao=A" method="post" name="formulario" id="formulario"> 
		<input type="hidden" id="salid" name="salid" value="<?=$salid?>">
		<input type="hidden" id="requisicao" name="requisicao" value="gravarsala">
		<table align="center" width="95%" cellSpacing="1" cellPadding="3" class="tabela" bgcolor="#f5f5f5" >
	      <tr>
	        <td align='right' class="SubTituloDireita" width="15%">C&oacute;digo da sala:</td>
	        <td colspan="2"><?= campo_texto('salcodigosala','N','N','',13,10,'','','','','','','','');?></td>
	      </tr>
	      
   	      <tr>
	        <td align='right' class="SubTituloDireita">Ano de cria&ccedil;&atilde;o:</td>
	        <td colspan="2">
	        	<?=campo_texto('salanocriacao_ano','S',(isset($salid)? 'N':'S'),'',7,4,'','');?>
	        </td>
	      </tr>
	      
	      <tr>
	        <td align='right' class="SubTituloDireita" colspan="3">&nbsp;</td>
	      </tr>	      
	      
   	      <tr>
	        <td align='right' colspan="3"></td>
	      </tr>	      
	      
   	      <tr id="transf_1" style="visibility: collapse;">
			<td align='right' class="SubTituloEsquerda" width="15%">Escola de origem:</td>
	        <td align='right' class="SubTituloEsquerda" width="85%" colspan="2">&nbsp;</td>
	      </tr>

      
   	      <tr>
	        <td align='right' class="SubTituloDireita">INEP:</td>
	        <td >
	        	<?
	        	$entcodent = $_REQUEST['entcodent'];
	        	echo campo_texto('entcodent','N','S','',13,10,'','','','','','','',$entcodent,'recuperarEntidadeEscola(this.value)');
	        	?>
	        	<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.'>
	        </td>
	      </tr>
	      
   	      <tr>
	        <td align='right' class="SubTituloDireita">Nome da escola:</td>
	        <td colspan="2">
	        	<input type="hidden" id="entid" name="entid" value="">
	        	<div id="entnome"></div>
	        </td>
	      </tr>	 
	      			
	      <tr id="transf_2" style="visibility: collapse;">
			<td align='right' class="SubTituloEsquerda" >Escola de destino:</td>
	        <td align='right' class="SubTituloEsquerda" colspan="2">&nbsp;</td>
	      </tr>
	      
   	      <tr id="transf_3" style="visibility: collapse;">
	        <td align='right' class="SubTituloDireita">INEP:</td>
	        <td colspan="2">
	        	<?
	        	$entcodent_destino = $_REQUEST['entcodent_destino'];
	        	echo campo_texto('entcodent_destino','N','S','',13,10,'','','','','','','',$entcodent,'recuperarEntidadeEscolaDestino(this.value)');
	        	?>
	        	<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.'>
	        </td>
	      </tr>
	      
   	      <tr id="transf_4" style="visibility: collapse;">
	        <td align='right' class="SubTituloDireita">Nome da escola:</td>
	        <td colspan="2">
	        	<input type="hidden" id="entid_destino" name="entid_destino" value="">
	        	<div id="entnome_destino"></div>
	        </td>
	      </tr>	 
	      			
	      <tr id="transf_5" style="visibility: collapse;">
	        <td align='right' class="SubTituloDireita">Data de transfer&ecirc;ncia:</td>
	        <td>
				<?= campo_data( 'tardatatransferencia','N', 'S', '',''); ?>
	        </td>
	      </tr>	
	      
	      <tr>
	      	<td align='right' class="SubTituloEsquerda"></td>
			<td id="td1" style="display: none;" align='right' class="SubTituloEsquerda">
	        	<input type="Button" style="width:170px;visibility: collapse;" id="gravarTransferencia" name="gravarTransferencia" value="Gravar Transfer&ecirc;ncia" onclick="javascript:gravarTransferenciaExec();">
			</td>
	        <td id="td2" align='right' class="SubTituloEsquerda" colspan="2">
				<? if( isset($salid) ) { ?>
	        			<input  type="Button" style="width:80px;" id="exibirTransferir" style="visibility: visible;" 
	        					name="Transferir" value="Transferir" onclick="javascript:exibirCamposTransferir();">
				<? }  ?>
	        </td>
	      </tr>
	      
	      <tr>
	        <td align='right' colspan="3"></td>
	      </tr>	      

	      <tr>
	        <td align='right' class="SubTituloDireita">Esfera:</td>
	        <td colspan="2">
				<?
				$sql2 = "select esfid as CODIGO, esfdescricao as DESCRICAO from salasrecmult.esferas order by DESCRICAO ";
				$db->monta_combo("esfid", $sql2, 'S' , 'Escolha uma esfera','','');
				?>	        
				<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.'>
	        </td>
	      </tr>	

	      <tr>
	        <td align='right' class="SubTituloDireita">Tipo:</td>
	        <td colspan="2">
				<?
				$sql2 = "select tipid as CODIGO, tipdescricao as DESCRICAO from salasrecmult.tipos order by tipid ";
				$db->monta_combo("tipid",$sql2,'S',"Escolha um tipo",'','');
				?>	        
				<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.'>
	        </td>
	      </tr>			

	      <tr>
	        <td align='right' class="SubTituloDireita">Situa&ccedil;&atilde;o:</td>
	        <td colspan="2">
				<?
				$sql2 = "select sitid as CODIGO, sitdescricao as DESCRICAO from salasrecmult.situacoes order by sitid ";
				$db->monta_combo("sitid",$sql2,'S',"Escolha uma situação",'','');
				?>	        
	        </td>
	      </tr>
	      
	      <tr>
	        <td align='right' class="SubTituloDireita">Justificativa:</td>
	        <td colspan="2">
		        <?=campo_textarea('salobservacao','N','S','',50,3,255,'','','',false)?>
	        </td>
	      </tr>	      

   	      <tr>	      
			<td class="SubTituloDireita" style="text-align:center;" width="18%">
				&nbsp;
			</td>
			<td class="SubTituloDireita" style="text-align:left;" colspan="2">
				<input type="Button" style="width:80px;" name="Gravar" value="Gravar" onclick="javascript:gravarSala();">
			</td>
	      </tr>
		</table>
	</form>

   	<form action="?modulo=principal/cadsala&acao=A"  enctype="multipart/form-data" method="post" name="formularioDocumento" id="formularioDocumento">	    
		<input type="hidden" id="salid" name="salid" value="<?=$salid?>">
		<input type="hidden" id="requisicao" name="requisicao" value="inseriranexo">
		<table align="center" width="95%" cellSpacing="1" cellPadding="3" class="tabela" bgcolor="#f5f5f5" >
	    <tr>
	        <td align='right' class="SubTituloDireita" width="18%">Enviar um arquivo:</td>
	        <td colspan="2">
				<input type="hidden" id="requisicao" name="requisicao" value="inseriranexo">
				<? if( isset($salid) ) { ?>
					<input type="file" name="arquivo" value="Arquivo" id="arquivo" size="55" maxlength="150" />
				<? } else { ?>
					<span>(Grave a sala primeiramente)</span>
				<? }  ?>
				<input type="Button" style="width:80px;" name="gravarAnexo" value="Gravar anexo" onclick="enviarArquivo();">
	        </td>
	    </tr>
   	      <tr>
   			<td class="SubTituloDireita" style="text-align:center;" width="18%">
				&nbsp;
			</td>
	      </tr>	    
		</table>
		
		<br />
		<?php

		$sql = "SELECT 	anexo.anxid, 
						anexo.arqid, 
						arqui.arqnome as nome,
						arqui.arqextensao as extensao
				FROM    salasrecmult.anexossalas anexo
				INNER JOIN public.arquivo arqui 
				ON anexo.arqid = arqui.arqid
				WHERE anexo.salid = $salid ";
		
		if( $salid ) {
			$anexos = $db->carregar($sql);
			if($anexos[0]) {
			?>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" class="tabela" align="center">
				<tr style="background-color: #e0e0e0">
					<td style="font-weight: bold; text-align: center; width: 10%">&nbsp;</td>
					<td style="font-weight: bold; text-align: center; width: 70%">Arquivo</td>
					<td style="font-weight: bold; text-align: center; width: 20%">Download</td>
				</tr>
					<?php foreach($anexos as $anexo) { ?>
					<tr>
						<td style="font-weight: bold; text-align: center; width: 10%">
							<img align="absmiddle"
								src="../imagens/excluir.gif"
								onmouseover="this.style.cursor='pointer'"
								onclick="excluir('?modulo=principal/cadsala&acao=A&salid=<?=$salid?>&requisicao=removeranexo&anxid=<?=$anexo['anxid']?>&arqid=<?=$anexo['arqid']?>','Deseja realmente excluir o anexo?');"
								title="excluir anexo" />
						</td>
						<td><? echo $anexo['nome'].".".$anexo['extensao']; ?></td>
						<td><? 
							echo "<a href=\"javascript:void(0);\" 
									 onclick=\"window.location='?modulo=principal/cadsala&acao=A&salid={$salid}&requisicao=download&arqid={$anexo['arqid']}'; \">
									 <img border=\"0\" src=\"../imagens/salvar.png\"> "
								 ."</a><br />"; 
						?></td>
					</tr>
					<?php } ?>
			</table>
			<?php } ?>		
		<?php } ?>
		
	</form>

	<table align="center" width="95%" border="0" cellpadding="2" cellspacing="0" class="listagem" style="color:#333333">
		<tbody>
			<?
			$sql_lista = "
					select 
					   to_char(transferencias.tardatatransferencia,'dd/mm/yyyy') as data_transferencia,
					   bentorigem.entcodent	  as origem_inep,
					   bentorigem.entnome     as origem_nome_escola,
					   bentdestino.entcodent  as destino_inep,
					   bentdestino.entnome    as destino_nome_escola
					  
					from 
					   salasrecmult.salas as salas 
					
					   inner join salasrecmult.transferencias as transferencias
					   on salas.salid = transferencias.salid
					   
					   left join entidade.entidade bentorigem
					   on transferencias.entidorigem = bentorigem.entid
					
					   left join entidade.entidade bentdestino
					   on transferencias.entiddestino = bentdestino.entid
					where
			";

			if( is_combo_set('salid') ) {
				$salid = $_REQUEST['salid']; 
				$sql_lista .= " salas.salid = $salid ";
			} else {
				$sql_lista .= " 0=1 ";	
			}
				   
			$cabecalho = array("Data da transferência","INEP Origem","Nome da escola de origem","INEP Destino","Nome da escola de destino");
			$db->monta_lista($sql_lista,$cabecalho,60,20,'','','');			
			?>
		</tbody>
	</table>	
<?
function removerAnexo($salid,$anxid,$arqid) {
	global $db;
	$sql = "DELETE FROM salasrecmult.anexossalas WHERE anxid=$anxid";
	$db->executar($sql);
	$sql = "DELETE FROM public.arquivo WHERE arqid=$arqid";
	$db->executar($sql);
	$db->commit();
	echo "<script>
			alert('Anexo removido com sucesso.');
			window.location='?modulo=principal/cadsala&acao=A&salid=$salid';</script>";
	exit;
}

//function gravarTransferencia( $salid, $entid , $saldataadesao , $saldataencerramento , $entid_destino , $saldataadesao_destino , $saldataencerramento_destino) {
//	global $db;
//	
//	$sqlNextVal = "SELECT nextval('salasrecmult.transferencias_traid_seq1'::regclass) as id";
//	$nextVal = $db->pegaUm($sqlNextVal);
//	
//	$sql = "INSERT INTO salasrecmult.transferencias 
//			(traid, salid, tardatatransferencia, entidorigem, entiddestino)
//			VALUES
//			( $nextVal,
//			  $salid, 
//			  NOW(),
//			  $entid,
//			  $entid_destino
//			  ) ";
//	$gravouTransferencia = ($db->executar($sql) != false);
//
//	if( $gravouTransferencia ) {
//		$sqlb = "UPDATE salasrecmult.salas SET
//				   entid = $entid_destino
//				 WHERE salid = $salid 
//				";
//		$db->executar($sqlb);
//		$db->commit();
//	} else {
//		$db->rollback();
//	}
//}

function gravarTransferencia( $salid , $entid , $entid_destino , $saldatatransferencia) {
	global $db;
	
	$sqlNextVal = "SELECT nextval('salasrecmult.transferencias_traid_seq'::regclass) as id";
	$nextVal = $db->pegaUm($sqlNextVal);

	$data = formata_data_sql( $saldatatransferencia );
	
	$sql = "INSERT INTO salasrecmult.transferencias 
			(traid, salid, tardatatransferencia, entidorigem, entiddestino)
			VALUES
			( $nextVal,
			  $salid, 
			  '$data',
			  $entid,
			  $entid_destino
			  ) ";
	$gravouTransferencia = ($db->executar($sql) != false);

	if( $gravouTransferencia ) {
		$sqlb = "UPDATE salasrecmult.salas SET
				   entid = $entid_destino
				 WHERE salid = $salid 
				";
		$db->executar($sqlb);
		$db->commit();
	} else {
		$db->rollback();
	}
}

function is_combo_set($campo='') {
	$auxiliar = $_REQUEST[$campo];
	return isset($auxiliar) && $auxiliar != "" && $auxiliar != "x";
} 

function add_clausula_select($campo_formulario,$campo_base_dados,$is_string=true) {
	if ( is_combo_set($campo_formulario) ) {
		$parametro = $_REQUEST[$campo_formulario];
		return " $campo_base_dados = " . ($is_string ? "'":"") . $parametro . ($is_string ? "'":"") . " and ";
	} else {
		return " ";
	}
}

function inserirAnexoSala($salid, $arqid) {
	global $db;	
	$sql = "INSERT INTO salasrecmult.anexossalas (
            salid, anxdataanexo, anxstatus, arqid)
    		VALUES ($salid, NOW(), 'A', $arqid);";
	$db->executar($sql);
}

function inserirArquivo() {
	global $db;
	// obtém o arquivo
	$arquivo = $_FILES['arquivo'];
	
	if ( !is_uploaded_file( $arquivo['tmp_name'] ) ) {
		echo "<script>alert('Erro no upload. Entre em contato com a equipe técnica');</script>";
		exit;
	}
	// BUG DO IE
	// O type do arquivo vem como image/pjpeg
	if($arquivo["type"] == 'image/pjpeg') {
		$arquivo["type"] = 'image/jpeg';
	}
	
	//Insere o registro do arquivo na tabela public.arquivo
	$sql = "INSERT INTO public.arquivo (arqnome,arqextensao,arqtipo,arqtamanho,arqdata,arqhora,usucpf,sisid)
	values('".current(explode(".", $arquivo["name"]))."','".end(explode(".", $arquivo["name"]))."','".$arquivo["type"]."','".$arquivo["size"]."','".date('Y-m-d')."','".date('H:i:s')."','".$_SESSION["usucpf"]."',". $_SESSION["sisid"] .") RETURNING arqid;";
	$arqid = $db->pegaUm($sql);
	if(!is_dir('../../arquivos/salasrecmult/'.floor($arqid/1000))) {
		mkdir(APPRAIZ.'/arquivos/salasrecmult/'.floor($arqid/1000), 0777);
	}
	$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arqid/1000) .'/'. $arqid;
	switch($arquivo["type"]) {
		case 'image/jpeg':
			ini_set("memory_limit", "128M");
			list($width, $height) = getimagesize($arquivo['tmp_name']);
			$original_x = $width;
			$original_y = $height;
			// se a largura for maior que altura
			if($original_x > $original_y) {
  	 			$porcentagem = (100 * 640) / $original_x;      
			}else {
   				$porcentagem = (100 * 480) / $original_y;  
			}
			$tamanho_x = $original_x * ($porcentagem / 100);
			$tamanho_y = $original_y * ($porcentagem / 100);
			$image_p = imagecreatetruecolor($tamanho_x, $tamanho_y);
			$image   = imagecreatefromjpeg($arquivo['tmp_name']);
			imagecopyresampled($image_p, $image, 0, 0, 0, 0, $tamanho_x, $tamanho_y, $width, $height);
			imagejpeg($image_p, $caminho, 100);
			//Clean-up memory
			ImageDestroy($image_p);
			//Clean-up memory
			ImageDestroy($image);
			break;
		default:
			if (!move_uploaded_file( $arquivo['tmp_name'], $caminho ) ) {
				$db->rollback();
				echo "<script>alert(\"Problemas no envio do arquivo.\");</script>";
				exit;
			}
	}
	//echo "<script>document.formulario.reset();document.formularioDocumento.reset();</script>";
	return $arqid;
}

if( $salid ) {
	$sql = " SELECT 
				sala.salcodigosala,
				enti.entid,
				enti.entcodent as entidadecod,
				enti.entnome,
				sala.salanocriacao,
				esfe.esfid,
				tipo.tipid,
				situ.sitid,
				sala.salobservacao
			 FROM
			 	salasrecmult.salas sala
			 	
			 	inner join salasrecmult.esferas esfe
			 	on sala.esfid = esfe.esfid
				 	
				inner join salasrecmult.tipos tipo
				on sala.tipid = tipo.tipid

				inner join salasrecmult.situacoes situ
				on sala.sitid = situ.sitid
				
				inner join entidade.entidade enti
				on sala.entid = enti.entid
			WHERE
				sala.salid = $salid "; 
	
	$dados = $db->pegaLinha($sql);	  
	$salcodigosala = $dados['salcodigosala'];
	$entid = $dados['entid'];
	$entcodentb = $dados['entidadecod'];
	$entnome = $dados['entnome'];
	$salanocriacaob = $dados['salanocriacao'];
	$esfid = $dados['esfid'];
	$tipid = $dados['tipid'];
	$sitid = $dados['sitid'];
	$salobservacao = $dados['salobservacao'];
	
	echo 
		"
		<script>
			$('formulario').salid.value = '$salid';
			$('formulario').salcodigosala.value = '$salcodigosala';  
			$('formulario').entid.value = '$entid';
			$('formulario').entcodent.value = '$entcodentb';
			$('entnome').innerHTML = '$entnome';
			$('formulario').salanocriacao_ano.value = '$salanocriacaob';
			$('formulario').esfid.value = '$esfid';
			$('formulario').tipid.value = '$tipid';
			$('formulario').sitid.value = '$sitid';
			$('formulario').salobservacao.value = '$salobservacao';
		</script>
		";
} else {
	$sqlb = " select nextval('salasrecmult.salas_salid_seq'::regclass) as id";
	$nextval = $db->pegaUm($sqlb);		
	
	
	$sql = "SELECT s.salcodigosala FROM salasrecmult.salas s WHERE s.salid = (SELECT MAX(x.salid) FROM salasrecmult.salas x)";
	$nextval = $db->pegaUm($sql);
	$tam = strlen( $nextval ) - 5;		
	$nextval = substr( $nextval, 0, $tam ) + 1;

	$tamanho = 5-sizeof($nextval); 
	for( $i=$tamanho-1; $i!=0; $i-- ) {
		$salcodigosala .= "0";
	}
	$salcodigosala .= $nextval;

	echo 
		"
		<script>
			$('formulario').salcodigosala.value = '$salcodigosala';  
		</script>
		";
}
?>

