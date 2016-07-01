<?php
include_once APPRAIZ . "includes/classes/dateTime.inc";

//verifica sessão da pagina $_SESSION['ctrid']
verificaSessaoPagina();

if($_REQUEST['download'] == 'S'){
	require_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
	
	$file = new FilesSimec();
	$arqid = $_REQUEST['arqid'];
	$arquivo = $file->getDownloadArquivo($arqid);
	echo"<script>window.location.href = 'contratos.php?modulo=principal/cadGarantia&acao=A';</script>";
	exit;
}

if($_REQUEST['arqidDel'] != ''){
	$anexos = array();
	$sql = "UPDATE contratos.ctanexo SET ancstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
	$db->executar($sql);
	$sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
	$db->executar($sql);

	$db->commit();
	echo"<script>alert('Operação realizada com sucesso!');window.location.href = 'contratos.php?modulo=principal/cadGarantia&acao=A';</script>";
}

function salvar(){
	global $db; 
 	 if($_POST['mogid'] == 7){
 	 	$formataDatamI = formata_data_sql( $_REQUEST['ctrdtinigar2'] );
 	 	$formataDatamI = formata_data_sql( $_REQUEST['ctrdtfimgar2'] );
 	 	$valor = $_REQUEST['ctrvlr2'];
 	 	$obsGarantia = $_POST['ctrobsgar2'];
 	 	$obsGeral = $_POST['ctrobservacao2'];
 	 }else{
 	 	$formataDatamI = formata_data_sql( $_REQUEST['ctrdtinigar'] );
 	 	$formataDatamF = formata_data_sql( $_REQUEST['ctrdtfimgar']); 
 	 	$valor = $_REQUEST['ctrvlr'];
 	 	$obsGarantia = $_POST['ctrobsgar'];
 	 	$obsGeral = $_POST['ctrobservacao'];
 	 }
	$ctrvlr = str_replace(',','.',str_replace('.','',$valor));
	if(!$ctrvlr) $ctrvlr = 'NULL';
	if(!$formataDatamI) $formataDatamI = NULL;
	if(!$formataDatamF) $formataDatamF = NULL;
	else $ctrvlr = "'".$ctrvlr."'";	
	
	
	$sql = "INSERT INTO contratos.historicogarantiacontrato(
	            ctrid, mogid, hgcvlrgarantia, hgcdataini, hgcdatafim, hgcobsgarantia, hgcobsgeral
			)(
				SELECT
					ctrid,
					mogid,
					ctrvlrgarantia,
					ctrdtinigar,
					ctrdtfimgar,
					ctrobsgar,
					ctrobservacao
				FROM
					contratos.ctcontrato
				WHERE
					ctrid = ".$_SESSION['ctrid']."
			);";

	$db->executar($sql);
	
	$sql = "UPDATE contratos.ctcontrato 
			SET
				ctrvlrgarantia	= ".$ctrvlr.",
				mogid			= ".$_POST['mogid'].",
				ctrdtinigar		= ";
	if($formataDatamI) $sql.="'".$formataDatamI."',"; else $sql.="NULL,";	
	$sql .="	ctrdtfimgar		="	;	
	if($formataDatamF) $sql.="'".$formataDatamF."',"; else $sql.="NULL,";
	$sql .=	"	ctrobsgar		= '".$obsGarantia."',
				ctrobservacao	= '".$obsGeral."'
			WHERE
				ctrid = ".$_SESSION['ctrid']."
				RETURNING ctrid
	"; 
		
 	//dbg($sql,1);
 	
	$ctrid = $db->pegaUm($sql);

	/*******************
	SALVAMENTO DOS ANEXOS - início
 	********************/	
	require_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
	
	$campos	= array("ctrid"				=> $_SESSION['ctrid'],
					"ancdatainclusao" 	=> "now()" ,
					"usucpf"    		=> "'" . $_SESSION['usucpf'] . "'",
					"tpaid"     		=> ID_GARANTIA_CONTRATO,
					"ancstatus" 		=> "'A'"
					);
	 
	if($_FILES['arquivo']['tmp_name']){
		foreach($_FILES['arquivo']['tmp_name'] as $n => $arquivo){
			if($_FILES['arquivo']['tmp_name'][$n]){
				$_FILES['arquivo_'.$n]['tmp_name'] 	= $_FILES['arquivo']['tmp_name'][$n];
				$_FILES['arquivo_'.$n]['type'] 		= $_FILES['arquivo']['type'][$n];
				$_FILES['arquivo_'.$n]['error'] 	= $_FILES['arquivo']['error'][$n];
				$_FILES['arquivo_'.$n]['size'] 		= $_FILES['arquivo']['size'][$n];
				$_FILES['arquivo_'.$n]['name'] 		= $_FILES['arquivo']['name'][$n];
				$file = new FilesSimec("ctanexo", $campos ,"contratos");
				$file->setUpload(($_POST['arqdescricao'][$n] ? $_POST['arqdescricao'][$n] : ""),'arquivo_'.$n,true,false);
			}
		}
	}
	/*******************	
	SALVAMENTO DOS ANEXOS - fim	
	********************/
	
	$db->commit();
	$_REQUEST['acao'] = "A";
	$db->sucesso('principal/cadGarantia', "");
	die;
} 


// if($_REQUEST['download'] == 'S'){
// 	$file = new FilesSimec();
// 	$arqid = $_REQUEST['arqid'];
// 	$arquivo = $file->getDownloadArquivo($arqid);
// 	echo"<script>window.location.href = 'contratos.php?modulo=principal/cadContratoAnexo&acao=A';</script>";
// 	exit;
// }


// if($_REQUEST['arqidDel'] != ''){
		
// 	$anexos = array();
// 	$sql = "UPDATE contratos.ctanexo SET ancstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
// 	$db->executar($sql);
// 	$sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
// 	$db->executar($sql);

// 	$db->commit();
// 	echo"<script>window.location.href = 'contratos.php?modulo=principal/cadContratoAnexo&acao=A';</script>";

// }



if($_POST['action']){
	salvar();
}

// busca dadas para edição			
$sql = "SELECT      		
	    		ctrvlrgarantia as ctrvlr,
       			ctr.mogid, 
       			to_char(ctr.ctrdtinigar::date,'YYYY-MM-dd') AS ctrdtinigar,
       			to_char(ctr.ctrdtfimgar::date,'YYYY-MM-dd') AS ctrdtfimgar,       		
       			ctr.ctrobsgar,
       			ctr.ctrobservacao
  		FROM contratos.ctcontrato ctr 
		WHERE
			ctr.ctrid = '".$_SESSION['ctrid']."'
			";
$rsDadosContrato = $db->carregar( $sql );
	
// Por padrão os campos vem habilitados
// $desabilitado   = false;
// $somenteLeitura = 'S';
// $verifica = true;
$acesso 		= verificaPermissaoTelaUsuario();
$desabilitado   = $acesso['desabilitado'];
$somenteLeitura = $acesso['leitura'];


$perfis 			= arrayPerfil();
// $arPerfisBloqEdicao = array(PERFIL_GESTOR_FINANCEIRO_UNIDADE,
// 		PERFIL_CONSULTA_UNIDADE,
// 		PERFIL_CONSULTA_GERAL);

// $arInt = array_intersect($perfis, $arPerfisBloqEdicao);

// //MÉTODO PARA A VERIFICAR SE O CONTRATO PERTENCE À UNIDADE DO USUARIO E SE O USUARIO PODERÁ EDITAR NESTA TELA
// if($_SESSION['ctrid'])
// 	$verifica = verificaResponsabilidade($_SESSION['ctrid'],$perfis,array(PERFIL_EQUIPE_TECNICA_UNIDADE));
// //verifica se nao esta no bloqueado e se caso nao esteja bloq ele tem direito a editar
// if( count($arInt) != 0 || (count($arInt) == 0 && $verifica==false)) {
// 	$desabilitado 	= true;
// 	$somenteLeitura = 'N';
// }



//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, obrigatorio() . ' Indica campo obrigatório' );
montaCabecalhoContrato($_SESSION['ctrid']);

?> 
<script src="../includes/prototype.js"></script>
<script src="../includes/calendario.js"></script> 
    <form name="formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario" enctype="multipart/form-data">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        	<tr>
                <td class="SubTituloDireita" align="right" width="25%">Modalidade: </td> 
                <td> 
                 <?
                 $sql = "SELECT 
                            mogid AS codigo, 
                            mogdsc AS descricao
                        FROM
                            contratos.ctmodalidadegarantia where mogstatus='A'order by codigo";
				 $mogid = $rsDadosContrato[0]['mogid'];
				 
				 $db->monta_combo('mogid', $sql, (($desabilitado)?'N':'S'), "Selecione...", '', '', '', '200', 'S', 'mogid',false,null,'Modalidade');
                 ?>
                </td>
            </tr>
            
            <tr class="campoobrigatorio" <?php if($mogid==7)echo 'style="display:none;"';?>>
                <td class ="SubTituloDireita" align="right">Valor:</td>
                <td>
                 <?
                 $ctrvlr = $rsDadosContrato[0]['ctrvlr'];  
                 if($ctrvlr) $ctrvlr = number_format($ctrvlr,2,",",".");                             
                 ?>   
                 <?= campo_texto( 'ctrvlr', 'S', $somenteLeitura, 'Valor', 17, 15, '###.###.###,##', '', 'right', '', 0, 'id="ctrvlr" onblur="MouseBlur(this);"'); ?>       
               
                </td>
                         
            </tr>
            <tr class="camponaoobrigatorio" <?php if($mogid!=7 || !$mogid)echo 'style="display:none;"';?>>
                <td class ="SubTituloDireita" align="right">Valor:</td>
                <td>
                 <?
                 $ctrvlr2 = $rsDadosContrato[0]['ctrvlr'];  
                 if($ctrvlr2) $ctrvlr2 = number_format($ctrvlr2,2,",",".");                             
                 ?>   
                 <?= campo_texto( 'ctrvlr2', 'N', $somenteLeitura, 'Valor', 17, 15, '###.###.###,##', '', 'right', '', 0, 'id="ctrvlr" onblur="MouseBlur(this);"'); ?>       
               
                </td>
                         
            </tr>
             
        	<tr class="campoobrigatorio" <?php if($mogid==7)echo 'style="display:none;"';?>>
                <td class ="SubTituloDireita" align="right" valign="top">Observação da Garantia: </td> 
                <td> 
                 <?
				 $ctrobsgar = $rsDadosContrato[0]['ctrobsgar'];
                 ?>
            	 <?//= campo_texto('ctrobsgar', 'S', $somenteLeitura, 'Observação da Garantia', 100, 200, '', '', 'left', '',  0, 'id="ctrobsgar" onblur="MouseBlur(this);"' ); ?>
	             <?= campo_textarea('ctrobsgar', 'S', (($desabilitado)?'N':'S'), 'Observação da Garantia', 73, 3, 200, '', '', '', '', 'Observação da Garantia'); ?>
                </td>
            </tr>
         	<tr class="camponaoobrigatorio" <?php if($mogid!=7 || !$mogid)echo 'style="display:none;"';?>>
                <td class ="SubTituloDireita" align="right" valign="top">Observação da Garantia: </td> 
                <td> 
                 <?
				 $ctrobsgar2 = $rsDadosContrato[0]['ctrobsgar'];
                 ?>
            	 <?//= campo_texto('ctrobsgar', 'S', $somenteLeitura, 'Observação da Garantia', 100, 200, '', '', 'left', '',  0, 'id="ctrobsgar" onblur="MouseBlur(this);"' ); ?>
	             <?= campo_textarea('ctrobsgar2', 'N', (($desabilitado)?'N':'S'), 'Observação da Garantia', 73, 3, 200, '', '', '', '', 'Observação da Garantia'); ?>
                </td>
            </tr>            
           
            <tr class="campoobrigatorio" <?php if($mogid==7)echo 'style="display:none;"';?>>
                <td class ="SubTituloDireita" align="right">Data de Início:</td>
                <td>
                <?php 
                $ctrdtinigar = $rsDadosContrato[0]["ctrdtinigar"];   
                ?>
                <?= campo_data2( 'ctrdtinigar','S', (($desabilitado)?'N':'S'), 'Data de início', 'S' ); ?>
                
                </td>
            </tr>
            <tr class="camponaoobrigatorio" <?php if($mogid!=7 || !$mogid)echo 'style="display:none;"';?>>
                <td class ="SubTituloDireita" align="right">Data de Início:</td>
                <td>
                <?php 
                $ctrdtinigar2 = $rsDadosContrato[0]["ctrdtinigar"];   
                ?>
                <?= campo_data2( 'ctrdtinigar2','N', (($desabilitado)?'N':'S'), 'Data de início', 'S' ); ?>
                
                </td>
            </tr>
            <tr class="campoobrigatorio" <?php if($mogid==7)echo 'style="display:none;"';?>>
                <td class ="SubTituloDireita" align="right">Data de Término</td>
                <td>
                <?php 
                $ctrdtfimgar = $rsDadosContrato[0]["ctrdtfimgar"];   
                ?>
                <?= campo_data2( 'ctrdtfimgar','S', (($desabilitado)?'N':'S'), 'Data de Término', 'S' ); ?>
                
                </td>
            </tr>
            <tr class="camponaoobrigatorio" <?php if($mogid!=7 || !$mogid)echo 'style="display:none;"';?>>
                <td class ="SubTituloDireita" align="right">Data de Término</td>
                <td>
                <?php 
                $ctrdtfimgar2 = $rsDadosContrato[0]["ctrdtfimgar"];   
                ?>
                <?= campo_data2( 'ctrdtfimgar2','N', (($desabilitado)?'N':'S'), 'Data de Término', 'S' ); ?>
                
                </td>
            </tr>
            <tr class="campoobrigatorio" <?php if($mogid==7)echo 'style="display:none;"';?>>
                <td class ="SubTituloDireita" align="right" valign="top">Observação Geral</td>
                <td>
                <?php 
                $ctrobservacao = $rsDadosContrato[0]["ctrobservacao"];   
                ?>
                <?= campo_textarea('ctrobservacao', 'S', (($desabilitado)?'N':'S'), 'Observação Geral', 73, 5, 2000, '', '', '', '', 'Observação Geral'); ?>
                
                </td>
            </tr>
            <tr class="camponaoobrigatorio" <?php if($mogid!=7 || !$mogid)echo 'style="display:none;"';?>>
                <td class ="SubTituloDireita" align="right" valign="top">Observação Geral</td>
                <td>
                <?php 
                $ctrobservacao2 = $rsDadosContrato[0]["ctrobservacao"];   
                ?>
                <?= campo_textarea('ctrobservacao2', 'N', (($desabilitado)?'N':'S'), 'Observação Geral', 73, 5, 2000, '', '', '', '', 'Observação Geral'); ?>
                
                </td>
            </tr>           
	    	<tr>
			    <td class="SubTituloDireita" valign="top" >
			        	Anexos:
				</td>
	    		<td align="left">
	    <?php if( !$desabilitado ):?>
	    			<table id="tb_anexo" class="tabela" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width:85%" >
	    				<thead>
	    					<tr>
	    						<th>Ação</th>
		    					<th>Anexo</th>
		    					<th colspan="2">Descrição</th>
		    					
	    					</tr>
	    				</thead>
	    				<tbody>						    
						    <tr id="tr_anexo">
						    	<td align="center">
						    		<img src='../imagens/excluir.gif' onclick="if($('#tb_anexo tbody tr').length > 1){ $(this).parent().parent().remove() } else { alert('Não é possível excluir mais linhas'); }" class='link' />
						    	</td>
						        <td><input type="file" name="arquivo[]" /></td>
						        <td>
					        	<?php 
						        	$arrAtributos = false;
						        	$arrAtributos['name'] = "arqdescricao[]";
						        	$arrAtributos['obrigatorio'] = false;
						        	$arrAtributos['habilitado'] = ($desabilitado ? false : true);
						        	$arrAtributos['size'] = 40;
						        	$arrAtributos['maxsize'] = 255;
						        	$arrAtributos['align'] = "left";
						        	echo campo_texto($arrAtributos)
					        	?>
						        </td>
					    		<td align="right">
					    			<img src="../imagens/gif_inclui.gif" onclick="addNovoAnexo(this)" class="link" />
					    			&nbsp;<span onclick="addNovoAnexo(this)" style="cursor:pointer;">Adicionar anexo</span>
					    		</td>
						    </tr>
					    </tbody>
					</table>
		<?php endif;?>					
 					<fieldset style="width: 80%; background: #FFFFFF;">
 						<legend>
 							Lista de anexos <font size="1">(Clique na imagem <img src="../imagens/anexo.gif"> para fazer download.)</font>
 						</legend>
	 					<?php 
				        $sql = "SELECT
				                        '<center>".(($desabilitado) ? "" : "<a style=\" cursor: pointer;\"onclick=\"javascript:excluirAnexo(' || arq.arqid || ');\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a>" ) . "</center>' as acao,                       
				                        to_char(arq.arqdata,'DD/MM/YYYY'),
				                        arq.arqdescricao,
				                        '<a style=\"cursor: pointer; color: blue;\" onclick=\"window.location=\'?modulo=principal/cadContratoAnexo&acao=A&download=S&arqid=' || arq.arqid || '\';\" />' || arq.arqnome || '.'|| arq.arqextensao ||'</a>',
				                        arq.arqtamanho || ' kbs' as tamanho ,
				                        arq.arqdescricao,                               
				                        usu.usunome
				                    FROM
				                        ((public.arquivo arq 
									INNER JOIN contratos.ctanexo anc ON arq.arqid = anc.arqid) 
									INNER JOIN contratos.tipoanexo tarq ON tarq.tpaid = anc.tpaid) 
									INNER JOIN seguranca.usuario usu ON usu.usucpf = arq.usucpf
				                    WHERE
				                        arq.arqstatus = 'A' AND 
										anc.ctrid = " . $_SESSION['ctrid'] . "
									ORDER BY
										ancid DESC";
						 
				        $cabecalho = array( "Ação",
				                            "Data Inclusão",
				                            "Descrição Arquivo",
				                            "Nome Arquivo",
				                            "Tamanho (Mb)",    
				                            "Responsável");
				        $db->monta_lista( $sql, $cabecalho, 50, 10, 'N', 'center', '' );
	 					?>		
					</fieldset>
				</td>
			</tr>            
                        
            <tr>
            	<td  class ="SubTituloDireita" align="right"></td>
            	 <td>
            	 	<? if(!$desabilitado) : ?>
            	   <input type="hidden" name="action" value="1" id="action">
            	   <input type="button" name="btnGravar" value="Gravar" id="btnGravar" onclick="enviaForm();">
            	   <? endif; ?>
            	  </td>
            </tr>
            
            <tr>
            	<td class="SubTituloCentro" colspan="2">Histórico de Garantias</td>
            <tr>
            <tr>
            	<td colspan="2">
            	<?php 
            	$sql = "SELECT
							mogdsc,
							hgcvlrgarantia,
							TO_CHAR(hgcdataini, 'DD/MM/YYYY') AS hgcdataini,
							TO_CHAR(hgcdatafim, 'DD/MM/YYYY') AS hgcdatafim,
							hgcobsgarantia,
							hgcobsgeral	
						FROM
							contratos.historicogarantiacontrato hc
						LEFT JOIN contratos.ctmodalidadegarantia mg ON mg.mogid = hc.mogid 
						WHERE
							ctrid = {$_SESSION['ctrid']}
            			ORDER BY
            				hgcid DESC";
            	
            	$arrCab = array("Modalidade","Valor","Data de início","Data de término", "Observação da garantia", "Observação geral");
            	$db->monta_lista($sql,$arrCab,30,10,"N","");
            	?>
            	</td>
            <tr>
      </table>
      </form>     

<script	type="text/javascript" src="../includes/JQuery/jquery-1.10.2.min.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript">
jQuery.noConflict();

jQuery(document).ready(function(){
   jQuery('#mogid').change(function(){
		if(jQuery(this).val()=='7'){
			jQuery('.camponaoobrigatorio').show();
			jQuery('.campoobrigatorio').hide();


		}else{
			jQuery('.campoobrigatorio').show();
			jQuery('.camponaoobrigatorio').hide();
		}
   });	
});

function downloadAnexo(arqid)
{
	window.location = '?modulo=principal/cadGarantia&acao=A&download=S&arqid=' + arqid;
}

function excluirAnexo( arqid ){
	if ( confirm( 'Deseja excluir o documento?' ) ) {
 		location.href= window.location+'&arqidDel='+arqid;
 	}
}

function addNovoAnexo(obj)
{
	var tr_clone = jQuery('#tr_anexo').clone();
	
	jQuery('td:eq(3)', tr_clone).html('');
	jQuery('td:eq(1) input', tr_clone).val('');
	jQuery('td:eq(2) input', tr_clone).val('');

	tr_clone.insertAfter( jQuery('#tb_anexo > tbody > tr:last') );
}


function enviaForm(){
 	var nomeform 		= 'formulario';
	var submeterForm 	= true;
	var campos 			= new Array();
	var tiposDeCampos 	= new Array();
	
	campos[0] 			= "mogid";
	tiposDeCampos[0] 	= "select";
	if(jQuery('#mogid').val()!='7'){
		campos[1]			= "ctrvlr"; 
		campos[2]			= "ctrobsgar";		
		campos[3]			= "ctrdtinigar";
		campos[4]			= "ctrdtfimgar";	
		campos[5]			= "ctrobservacao";	
						 
		tiposDeCampos[1] 	= "valor";
		tiposDeCampos[2] 	= "textarea";
		tiposDeCampos[3] 	= "data";
		tiposDeCampos[4] 	= "data";
		tiposDeCampos[5] 	= "textarea";
	}
 
	

	validaForm(nomeform, campos, tiposDeCampos, submeterForm );
}

</script>