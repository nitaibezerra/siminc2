<?php

$covcod = $_REQUEST['covcod'];
$estuf = $_REQUEST['estuf'];

$download  = $_REQUEST['download'];
$arquivoid = $_REQUEST['arquivoid'];

#cadastra uma Formaçao Inicial
$somenteLeitura = 'S';

if ($download == "1" && !array_key_exists('btnGravar', $_REQUEST )){
		$sql ="SELECT * FROM public.arquivo WHERE arqid = ".$arquivoid;		
        $arquivo = $db->pegaLinha($sql);
        $caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arquivo['arqid']/1000) .'/'.$arquivo['arqid'];
        if ( !is_file( $caminho ) ) {
            $_SESSION['MSG_AVISO']= "Arquivo não encontrado.";
        }
        $filename = str_replace(" ", "_", $arquivo['arqnome'].'.'.$arquivo['arqextensao']);
        header( 'Content-type: '. $arquivo['arqtipo'] );
        header( 'Content-Disposition: attachment; filename='.$filename);
        readfile( $caminho );
        exit();
}

if ($_REQUEST['madid']) {
    
    $sql = "select * 
            from 
                parindigena.itemmonitoramento as im 
            inner join parindigena.materialdidatico as m on m.madid = im.madid 
            where m.madid = {$_REQUEST['madid']}
            and im.itmstatus = 'A'";
    
    $rsDadosMat = $db->carregar( $sql );
    
    $itmid 			   = $rsDadosMat[0]['itmid'];
    $estuf 		 	   = $rsDadosMat[0]['estuf'];
    $_REQUEST['estuf'] = $rsDadosMat[0]['estuf'];  
    
	$sql = "	SELECT DISTINCT apmp.sbaid,  coalesce(pss.pssano,0) as pssano
				FROM parindigena.associativaplanodemetasprogramas apmp
				INNER JOIN cte.projetosapesubacao pss on apmp.sbaid = pss.sbaid 
				WHERE appid = ".$_REQUEST['madid']." and apptipo = 3 ";
	
	$sub = $db->carregar( $sql );
	
	if($sub){
		
		$valoreTotalExecutado = 0;
		
		foreach($sub as $dados){
			
			$sbaids[] 	= $dados['sbaid']; 
			$pssanos[] 	= $dados['pssano'];
			
			$sql = "select sum(coalesce(hic.hmsvalortotalpago,0)) as total 
			from cte.historicomonitoramentoconvenio  hmc
			inner join  cte.historicomonitoramentoconvsubac hms on hmc.hmcid = hms.hmcid  
			inner join cte.historicoconvitemcomposicao 	hic on   hic.hmsid = hms.hmsid
			where hms.sbaid = ".$dados['sbaid']." and hic.hmsano = ".$dados['pssano'];
			
			$valoreTotalExecutado += $db->pegaUm( $sql );
		}
	}
}

if (array_key_exists('btnGravar', $_REQUEST ) ) {
	    
    if( $erro != 1 ){
    	    
        //insert das etapas.... 
		if (  $_REQUEST['maeid']){
             	
			for( $k = 0; $k < count( $_REQUEST['maeid'] ); $k++){
                           
				$arrDataI = explode( "/", $_REQUEST['maedatainicial'][$k]);
				$formataDataI = $arrDataI[2].'-'.$arrDataI[1].'-'.$arrDataI[0];
                
                $arrDataF = explode( "/", $_REQUEST['maedatafinal'][$k]);
                $formataDataF = $arrDataF[2].'-'.$arrDataF[1].'-'.$arrDataF[0];
                
                if ($_REQUEST['maeid'][$k] == '' &&  $formataDataI != '--' &&  $formataDataF != '--' && $_REQUEST['madcusto'][$k] != '' ){
 
                    if ($_REQUEST['madid'] != '' ){
                        $madid = $_REQUEST['madid'];
                    }
                    
					$sql = "select * from parindigena.materialdidaticoetapa where madid = '$madid' and etpid = '{$_REQUEST['etpid'][$k]}'";
					$rs = $db->carregar($sql);
                        
					if ( !$rs  ) {
                         
						$sql = "INSERT INTO parindigena.materialdidaticoetapa (
									maedatainicial,
									maedatafinal,
									etpid,
									madid,
									madcusto)
								VALUES ( 
									'{$formataDataI}',       
									'{$formataDataF}', 
   									'{$_REQUEST['etpid'][$k]}',
									'{$madid}',
									'{$_REQUEST['madcusto'][$k]}')";
                                             
					}else{
						$negado = true;
					}
                                                  
                }else{
                	
					if (  $formataDataI != '--' &&  $formataDataF != '--' && $_REQUEST['madcusto'][$k] != ''  ){
                        
                        $contEtapas = count($_REQUEST['etpid']);
                        $unique     = array_unique( $_REQUEST['etpid']);
                        $contUnique = count( $unique );
                        
                        //verificando se nao há nenhum valor de etapas da combo, sendo duplicado (regras do banco)
                        if ( $contUnique >= $contEtapas ){                 
                                                    
							$sql = "UPDATE 
                                        parindigena.materialdidaticoetapa 
                                    SET
                                            maedatainicial         = '{$formataDataI}',
                                            maedatafinal           = '{$formataDataF}', 
                                            etpid                  = '{$_REQUEST['etpid'][$k]}',
                                            madid                  = '{$_REQUEST['madid']}',
                                            madcusto               = '{$_REQUEST['madcusto'][$k]}'
                                    WHERE
                                        maeid     =  '{$_REQUEST['maeid'][$k]}' "; 
                      
                        }else{
							$negado = true;
                        }
                    }
                }
                
            $db->executar($sql);
            $db->commit();
            
            $dadosSalvos = true;
            }
            if ( $negado == true ){
                echo("<script>alert('Voce nao pode duplicar uma etapa de producao.');\n</script>");
            }
		}
	}
	
    echo '<script type="text/javascript">window.opener.location.reload(); window.close();</script>';  
                  
} //FIM DO INSERT PARA o MATERIAL DIDATICO.

if (array_key_exists('excluir', $_REQUEST)) {
	
    $sql = "DELETE FROM parindigena.materialdidaticoetapa WHERE maeid = {$_REQUEST['excluir']}";
 
    $deleteItemMat = $db->executar( $sql );
    $db->commit();
}

$sql = "select
		 covcod 
		from 
			parindigena.itemmonitoramento as im 
			inner join parindigena.materialdidatico as m 
		on m.madid = im.madid 
		 where 
			m.madid= {$_REQUEST['madid']} and 
			im.itmstatus = 'A'";

$rsDadosForm = $db->carregar( $sql );

$sql = "SELECT covnumero FROM financeiro.convenio WHERE covcod = '{$rsDadosForm[0]["covcod"]}'";
$rs = $db->carregar( $sql );
$convenioAno = $rs[0]['covnumero']; 
$dadosConvenio = explode('/', $convenioAno);
$numConvenio = $dadosConvenio[0];
$ano = $dadosConvenio[1];

$_REQUEST['covcod'] = $numConvenio;
$sql = "SELECT	 
			si.sbaid,
			si.sbaporescola
		FROM parindigena.associativaplanodemetasprogramas app
		inner join cte.subacaoindicador 	si ON si.sbaid = app.sbaid
		WHERE app.appid = '{$_REQUEST['madid']}'
		";
        
$arSubacoes = $db->carregar($sql);		

if(is_array($arSubacoes)){
	
	$totalValorProgramado = 0;
	foreach($arSubacoes as $dados){
		$arSbaids[] = $dados['sbaid'];
		$arValores = calculoValorProgramado($dados['sbaid'], $ano, $dados['sbaporescola']);
		$totalValorProgramado += $arValores['cronograma']; 	
	}
	
}

$totalValorProgramado = $totalValorProgramado ? $totalValorProgramado : 0;

$res = array( 0 => array (  "descricao" => "Identificação do material",
							    "id" 		=> "3",
							    "link" 		=> "/parindigena/parindigena.php?modulo=principal/popupCadMaterial&acao=I&madid={$rsDadosMat[0]['madid']}"
					  		  )
						);
				
if ($_REQUEST['madid']){
	
	$perfis = arrayPerfil();
	 		  
	if( in_array( PARIND_ADMINISTRADOR, $perfis ) || in_array( PARIND_SUPER_USUARIO, $perfis ) ){
			
		array_push($res,
					array ("descricao" => "Etapas",
							    "id"        => "0",
							    "link" 		=> "/parindigena/parindigena.php?modulo=principal/etapaCadMaterial&acao=A&madid={$rsDadosMat[0]['madid']}"
							   ),
					array ("descricao" => "Restrição",
							    "id"        => "1",
							    "link" 		=> "/parindigena/parindigena.php?modulo=principal/restricao&acao=A&madid={$rsDadosMat[0]['madid']}"
							   ),
		   			array ("descricao" => "Documentos",
							    "id"		=> "2",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/documento&acao=A&madid={$rsDadosMat[0]['madid']}"
					  		   ),
					array ("descricao" => "Vinculação a subações do PAR Genêrico",
							    "id"		=> "3",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/associativaPAR&acao=A&madid={$rsDadosMat[0]['madid']}&estuf={$_REQUEST['estuf']}"
		  		   				),
					array ("descricao" => "Subações do PAR - Plano de Metas",
							    "id"		=> "4",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/subacoesPAR&acao=A&madid={$rsDadosMat[0]['madid']}&estuf={$_REQUEST['estuf']}"
					  		   )
			  );
	}else{
		
		array_push($res,
					array ("descricao" => "Etapas",
							    "id"        => "0",
							    "link" 		=> "/parindigena/parindigena.php?modulo=principal/etapaCadMaterial&acao=A&madid={$rsDadosMat[0]['madid']}"
							   ),
					array ("descricao" => "Restrição",
							    "id"        => "1",
							    "link" 		=> "/parindigena/parindigena.php?modulo=principal/restricao&acao=A&madid={$rsDadosMat[0]['madid']}"
							   ),
		   			array ("descricao" => "Documentos",
							    "id"		=> "2",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/documento&acao=A&madid={$rsDadosMat[0]['madid']}"
					  		   ),
					array ("descricao" => "Subações do PAR - Plano de Metas",
							    "id"		=> "3",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/subacoesPAR&acao=A&madid={$rsDadosMat[0]['madid']}&estuf={$_REQUEST['estuf']}"
					  		   )
			  );
	}
	
}

echo montarAbasArray($res, $_REQUEST['org'] ? false : "/parindigena/parindigena.php?modulo=principal/etapaCadMaterial&acao=A&madid={$rsDadosMat[0]['madid']}");

include APPRAIZ."includes/arquivo.inc";

?>
<html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="-1">
    <title><?php echo $titulo ?></title>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script src="../includes/prototype.js"></script>
    <script src="../includes/entidades.js"></script>
    <script src="../includes/calendario.js"></script>
    
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

    <style type="text/css" media="screen">
        #loader-container,
        #LOADER-CONTAINER
        {
            background: none;
            position: absolute;
            width: 100%;
            text-align: center;
            z-index: 8000;
            height: 100%;
        }

        #loader {
            background-color: #fff;
            color: #000033;
            width: 300px;
            border: 2px solid #cccccc;
            font-size: 12px;
            padding: 25px;
            font-weight: bold;
            margin: 150px auto;
        }
        
	</style>
</head>
<body>
<form onSubmit="return validaForm(this);" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario" name="formulario" enctype="multipart/form-data">
<!-- Inicio da 1 tabela -->
	<table  align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
		<tr style="background-color: #cccccc;">
			<td>
				<div style="margin: 0; padding: 0;  width: 100%; background-color: #eee; border: none;">
				<table id="EtapasProducao" border="0" align="center" cellpadding="0" cellspacing="0" width="100%" style="text-align: center">            
<!-- Inicio da 5 tabela -->		
								<div id="loader-container">
									<div id="loader">
										<img src="../imagens/wait.gif" border="0" align="middle">
										<span>Aguarde! Carregando Dados...</span>
									</div>
								</div>
								<tr>
						<td colspan = "6" align="center" >
<!-- Inicio da 2 tabela -->						
							<table width = "100%" class="tabela listagem">
							<div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Identificação do Material</div>
								<tr>
									<td>
										<span style="text-align: left;display: block;font-size:100%;margin-bottom:5px;color:blue"><?php echo $_REQUEST['estuf']; ?></span>
									</td>
								</tr>
								<tr>
									<td>                                
										<span style="display: block;font-size:100%;color:blue"><img src="../imagens/seta_filho.gif">Material Didático</span>
									</td>
								</tr>                        
								<?php
								if ( $_REQUEST['covcod'] != '' )
								{                             
									$sql = "SELECT covnumero FROM financeiro.convenio WHERE covcod = '{$_REQUEST['covcod']}'";
									$rs = $db->carregar( $sql );
									$convenio = $rs[0]['covnumero'];                        
								?>
								<tr>
									<td>
										<span style="display: block;font-size:100%;color:black;"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="../imagens/seta_filho.gif"> Convênio(<?php  echo $convenio; ?>)</span>
									</td>
								</tr>
								<?php
								}
								if ( $_REQUEST['madid'] != '' )
								{                             
									$sql = "SELECT covnumero FROM financeiro.convenio WHERE covcod = '{$rsDadosMat[0]["covcod"]}'";
									$rs = $db->carregar( $sql );
									$convenio = $rs[0]['covnumero'];                        
								?>
								<tr>
									<td>
										<span style="display: block;font-size:100%;color:black;"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="../imagens/seta_filho.gif"> Convênio(<?php  echo $convenio; ?>)</span>
									</td>
								</tr>
								<tr>
									<td>
										<span style="display: block;font-size:100%;color:black; font-weight:bold;"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="../imagens/seta_filho.gif"> <?php  echo $rsDadosMat[0]["itmnome"]; ?></span>
									</td>
								</tr>
								<?php
								}
								?>
<!-- Fim da 2 tabela -->								
							</table>
						</td>
					</tr>
					<tr style="background-color: #cccccc;">
						<td colspan="6">
							<div class="SubtituloEsquerda" style="padding: 3px; text-align: center">Etapas</div>
							<div style="margin: 0; padding: 0; height: 250px; width: 100%; background-color: #eee; border: none;" class="div_rolagem">
<!-- Inicio da 6 tabela -->										
								<table id="itensEtapasProducao" border="0" align="center" cellpadding="0" cellspacing="0" width="100%" style="text-align: center">
									<colgroup>
										<col width="10%" />
										<col width="25%" />
										<col width="25%" />
										<col width="20%" />
										<col width="20%" />
									</colgroup>        
									<tr style="text-align: center">
										<td style="padding: 2px; font-weight: bold;">&nbsp;</td>
										<td style="padding: 2px; font-weight: bold;">Data de Inicio</td>
										<td style="padding: 2px; font-weight: bold;">Data Fim</td>
										<td style="padding: 2px; font-weight: bold;">Etapas de Produção</td>
										<td style="padding: 2px; font-weight: bold;">Custo da Etapa R$:</td>
									</tr>
<!-- Fim da 6 tabela -->												
								</table>
							</div>
							<div style="padding: 5px 0px 0px 5px"></div>        
							<span style="cursor:pointer" id="linkInserirItem" onclick="return adicionarEtapa('','','','','');"><img src="/imagens/gif_inclui.gif" align="middle" style="border: none" /> Inserir Etapas</span><br />
							        
						</td>
					</tr>								
					<?php
					$perfis = arrayPerfil(); 
					if( !in_array( PARIND_SUPER_USUARIO, $perfis ) &&
						!in_array( PARIND_ADMINISTRADOR, $perfis ) &&
						!in_array( PARIND_COORDENACAO, $perfis) &&
						!in_array( PARIND_EQUIPE_TECNICA, $perfis) )
					{
					?>
					<tr style="background-color: #cccccc;">
						<td class="SubTituloEsquerda"  align="left"> 
						</td>
					</tr> 
					<?php	
					} else {
					?>
					<tr style="background-color: #cccccc;">
						<td class="SubTituloEsquerda" colspan="2" align="left">     
							<input type="hidden" name="download"   value="0">
							<input type="hidden" name="arquivoid"  value="0">
							<input type="submit" name="btnGravar"  value="Gravar"/>
							<input type="button" name="btnCancel"  value="Fechar" onclick="fecharPopup();"/>
						</td>
					</tr> 	  
					<?php } ?>
					
<!-- Fim da 5 tabela -->
						<?php 
						
						if($sbaids){
							
							$sql = "select distinct
										app.sbaid,
										count(*) as total
									from parindigena.associativaplanodemetasprogramas app
									inner join cte.subacaoindicador si on si.sbaid = app.sbaid
									inner join cte.acaoindicador ai on ai.aciid = si.aciid
									inner join cte.pontuacao pt on pt.ptoid = ai.ptoid
									inner join cte.instrumentounidade iu on pt.inuid = iu.inuid
									where app.apptipo = '3'
									and iu.itrid = '1'
									and iu.estuf = '{$estuf}'									
									group by app.sbaid";
									
							$nrSubacoes = $db->carregar($sql);
							
							foreach($nrSubacoes as $dados){
								if($dados['total'] > 1){
									foreach($sbaids as $sbaid){
										if($sbaid == $dados['sbaid']){
											$mostraAlerta = 'true';
										}
									}
								}
							}
						}
						
						$mostraAlerta = $mostraAlerta ? $mostraAlerta : false;
						
						if($mostraAlerta){ 
						?>	
						<tr>
							<td  colspan="2" class="SubTituloDireita" >
							<span style="color:red;">
							<b>
							Os valores apresentados correspondem ao somatório de todas as subações do Material Didático.
							</b>
							</span>
							</td>										
						</tr>
						<?php } ?>
						<tr>
							<td  class="SubTituloDireita" >Valor total programado:</td>
							<td class="SubTituloEsquerda">R$ <?=number_format(str_replace(',','',$totalValorProgramado),2,',','.');?></td>
						</tr>
						<tr>
							<td  class="SubTituloDireita" >Valor total executado:</td>
							<td class="SubTituloEsquerda">R$ <?=number_format(str_replace(',','',$valoreTotalExecutado),2,',','.');?></td>
						</tr>  							
					</table>
				</div>
			</td>
		</tr>
	</table>
</form>
</body>
  
<script type="text/javascript">

function processaCombosPopup()
{

	selectAllOptions( document.getElementById( 'povid' ) );
	selectAllOptions( document.getElementById( 'linid' ) );
	selectAllOptions( document.getElementById( 'terid' ) );
    
    
	var numPovo   = document.getElementById( 'povid' ).value;
	var numLingua = document.getElementById( 'linid' ).value;
	var numTerri  = document.getElementById( 'terid' ).value;
	
	
	if( numPovo == '' )
	{
		alert('Você deve selecionar no mínimo um Povo Indígena.');
		return false;
	}
	if( numLingua == '' )
	{
		alert('Você deve selecionar no mínimo uma Lingua Indígena.');
		return false;
	}
	if( numTerri == '' )
	{
		alert('Você deve selecionar no mínimo um Território.');
		return false;
	}
    
}
  
  //Funçao que abre a popup para listar os laboratorios cadastrados.
 
function validaForm()
{
  
    var madautor = document.getElementById('madautor');
    marcado = -1
    
    if(formulario.covcod_disable.value == '')
    {
        alert('Selecione um convênio');
        formulario.covcod_disable.focus();
        return false;
    }
    if(formulario.itmnome.value == '')
    {
        alert('Selecione um Nome para o Material');
        formulario.itmnome.focus();
        return false;
    }
    if ((formulario.tipid.value == '') || (formulario.tipid.value == null))
    {
       alert('Selecione um Tipo de material');
       formulario.tipid.focus();
       return false;
    }
    
    if ((formulario.areid.value == '') || (formulario.areid.value == null))
    {
       alert('Selecione uma Área temática');
       formulario.areid.focus();
       return false;
    }
    if( madautor.value.length > 1000 ){
    	alert('Campo "Autor" muito longo maximo de 1000 caracteres.');
    	madautor.focus;
    	return false;
    }
    if ((formulario.madtiragem.value == '') || (formulario.madtiragem.value == null))
    {
       alert('Campo "tiragem" obrigatório');
       formulario.madtiragem.focus();
       return false;
    }
    if ((formulario.nivid.value == '') || (formulario.nivid.value == null))
    {
       alert('Selecione um Nível de Ensino');
       formulario.nivid.focus();
       return false;
    }
      	
	for (i=0; i<formulario.madavaliacaocapema.length; i++) {
	
		if (formulario.madavaliacaocapema[i].checked) {
			marcado = i			
		}
		
		if(marcado == -1){
		
			alert('Selecione uma avaliacao CAPEMA');        	
        	return false;
		}		
	}
    
    divElement=document.getElementById('itensEtapasProducao');
    inputs = divElement.getElementsByTagName('input');
    var datai;
    var dataf;
    for(i=0;i<inputs.length;i++)
    {
      if (inputs[i].name == 'maedatainicial[]'){
          datai = inputs[i]; 
      }
      if (inputs[i].name == 'maedatafinal[]'){
          dataf = inputs[i];
      } 
      if ((datai != null && datai.value != '') && (dataf != null && dataf.value !='') ){
          if (!validaDataMaior(datai, dataf)){
              alert('Data inicial da etapa nao pode ser maior que a data final');
              datai.focus();
              return false;
          }
      }
    }
    return processaCombosPopup();
}


 function fecharPopup()
{
    window.close();
}
 
    this._closeWindows = false;
    var _modificado    = false;

    /**
     * 
     */
    function removerEtapa(linha, maeid)
    {
        var linha = $(linha);
        linha.style.backgroundColor = 'rgb(255,255,210)';
        linha.style.backgroundColor = '#DAE0D2';

        if (!confirm('Atenção! Os dados serão apagados permanentemente!\nDeseja realmente excluir o item selecionado?')) {
            linha.style.backgroundColor = '#f0f0f0';
                 
        } else {
          $('loader-container').show();
            try {
                if (maeid != '') {
                    var exc = new Ajax.Request(window.location.href,
                                               {
                                                   method: 'post',
                                                   parameters: 'excluir=' + maeid,
                                                   onComplete: function(r)
                                                   {
                                                       //if (r.responseText == 'null')
                                                      
                                                           linha.parentNode.removeChild(linha);
                                                       //else
                                                       //    alert('Não foi possível excluir o item.');
                                                           linha.style.backgroundColor = '#f0f0f0';
                                                   }
                                               });
                } else {
                    linha.parentNode.removeChild(linha);
                }
            } catch (e) {
            }

            $('loader-container').hide();
        }

        return false;
    }

    
    /**
     * 
     */
    function adicionarEtapa(maeid, maedatainicial,  maedatafinal, etpid, madcusto)
    {
        var input_maedatainicial;
        var input_etpid;
        var input_maeid;
        var input_maedatafinal;
        var input_madcusto;
       
        var btnExcluirEtapa = document.createElement('img');
        btnExcluirEtapa.src     = '/imagens/excluir.gif';
        btnExcluirEtapa.onclick = function()
        {
            removerEtapa(this.parentNode.parentNode.getAttribute("id"), maeid);
        }

        //Data inicial
        input_maedatainicial    = document.createElement('input');
        input_maedatainicial.setAttribute('type', 'text');
        input_maedatainicial.setAttribute('class', 'normal');
        input_maedatainicial.setAttribute('name', 'maedatainicial[]');
        input_maedatainicial.setAttribute('maxlength', '10');
        input_maedatainicial.setAttribute('size', '12');
        input_maedatainicial.setAttribute('value', mascaraglobal('##/##/####',maedatainicial));
        input_maedatainicial.setAttribute('onKeyUp', 'this.value=mascaraglobal("##/##/####", this.value)');
        input_maedatainicial.setAttribute('onBlur', 'VerificaData(this, this.value)');
        //hidden, id da etapa
        input_maeid     = document.createElement('input');
        input_maeid.setAttribute('type', 'hidden');
        input_maeid.setAttribute('name', 'maeid[]');
        input_maeid.setAttribute('value', maeid);

        //combo das etapas
        input_etpid   = document.createElement('select');
        input_etpid.setAttribute('class', 'CampoEstilo');
        input_etpid.setAttribute('name', 'etpid[]');

        <?php
        
        //carregando a combo das etapas.
        $res = (array) $db->carregar('select etpid, etpdescricao from parindigena.etapaproducao order by etpdescricao');
    
        while (list(, $item) = each($res)) {
            $item = array_map('addslashes', $item);
            echo 'selected=etpid==' , $item["etpid"]
                , ';input_etpid.options[input_etpid.options.length]=new Option(\'' , $item["etpdescricao"] , '\',\'' , $item["etpid"] , '\',selected,selected);';
        }

       ?>

 
        input_maedatafinal    = document.createElement('input');
        input_maedatafinal.setAttribute('type', 'text');
        input_maedatafinal.setAttribute('class', 'normal');
        input_maedatafinal.setAttribute('name', 'maedatafinal[]');
        input_maedatafinal.setAttribute('maxlength', '10');
        input_maedatafinal.setAttribute('size', '12');
        input_maedatafinal.setAttribute('value', mascaraglobal('##/##/####',maedatafinal));
        input_maedatafinal.setAttribute('onKeyUp', 'this.value=mascaraglobal("##/##/####", this.value)');
        input_maedatafinal.setAttribute('onBlur', 'VerificaData(this, this.value)');
           
        input_madcusto    = document.createElement('input');
        input_madcusto.setAttribute('type', 'text');
        input_madcusto.setAttribute('class', 'normal');
        input_madcusto.setAttribute('name', 'madcusto[]');
        input_madcusto.setAttribute('maxlength', '100');
        input_madcusto.setAttribute('size', '20');
        input_madcusto.setAttribute('value', mascaraglobal('#.###.###.###,##',madcusto));
        input_madcusto.setAttribute('onKeyUp', 'this.value=mascaraglobal("#.##.###.###,##", this.value)');

        var tabelaEtapasProducao = $('itensEtapasProducao');
        var novaLinha                    = tabelaEtapasProducao.insertRow(tabelaEtapasProducao.rows.length);
        novaLinha.id                     = 'linhaitensEtapasProducao' + tabelaEtapasProducao.rows.length + 1;
        novaLinha.style.textAlign        = 'center'
        novaLinha.style.backgroundColor  = '#f0f0f0';

        /**
         * Botao excluir
         */
        var novaCelula0 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula0.appendChild(btnExcluirEtapa);

        /**
         * Data inicio
         */
        var novaCelula2 = novaLinha.insertCell(novaLinha.cells.length);
        input_maedatainicial.setAttribute('id', novaLinha.cells.length);
        novaCelula2.appendChild(input_maedatainicial);
        novaCelula2.appendChild(input_maeid);

        /**
         * Data fim
         */
        var novaCelula3 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula3.appendChild(input_maedatafinal);
         
        /**
         * Etapa de produçao
         */
        var novaCelula4 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula4.appendChild(input_etpid);
        
        
         /**
         * Custo
         */
        var novaCelula5 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula5.appendChild(input_madcusto);
    }

    function popup_arquivo( madid, nome, width, height )
		{
           
			window.open( '../geral/popup_arquivoupload.php?madid=' + madid + '&nome=' + nome, '', "height=" + height +  ",width=" + width +  ",scrollbars=yes,top=50,left=200" );
		}

    function carregarEtapasProducao(){<?php
        
    if( $_REQUEST['madid'] )
    {    
   
        $itens = $db->carregar('SELECT
                                    me.maeid,
                                    me.etpid,
                                    me.maedatainicial, 
                                    me.maedatafinal,
                                    me.madcusto
                                FROM
                                    parindigena.materialdidaticoetapa me
                                WHERE
                                    me.madid  = ' . $_REQUEST['madid'] . '
                                ORDER BY
                                    me.etpid ASC');
    }

    if (!is_array($itens)) 
    {
        echo 'return false;';
    } else {
        while (list(, $item) = each($itens)) {
            $item = array_map('addslashes', $item);
            
            $arrexpI = explode( "-", $item['maedatainicial']);
            $converteDataI = $arrexpI[2].'-'.$arrexpI[1].'-'.$arrexpI[0];
            
            $arrexpF = explode( "-", $item['maedatafinal']);
            $converteDataF = $arrexpF[2].'-'.$arrexpF[1].'-'.$arrexpF[0];
            
            echo "adicionarEtapa(", $item['maeid'] , ",'" , $converteDataI , "','" , $converteDataF , "'," , $item['etpid'] ,",'" , $item['madcusto'] , "');";
        }
    }

?>
}
<?php 
if ( $_REQUEST['madid'] ) 
{ 
?>
//    carregarAnexosAvaliacao();
    carregarEtapasProducao();
    $('loader-container').hide();
<?php } ?>    
 $('loader-container').hide();
function fecharJanela()
{
    if (_modificado) {
        if (!confirm('Existem dados que não foram salvos.\nDeseja fechar a janela sem salvá-los?'))
            return false;
    }

    window.close();
}

function adicionarAnexo(anxid, descricao, arqid, id)
    {
        
        var input_descricao;
        var input_anxid;
        var input_arqid;        
        var input_itmid;
        
        var linkExcluir = document.createElement('a');
        linkExcluir.setAttribute('href', '#');
        linkExcluir.onclick = function()
        {
            removerEtapa(this.parentNode.parentNode.getAttribute("id"), anxid);
        }
       	
        var btnExcluirEtapa = document.createElement('img');
        btnExcluirEtapa.src     = '/imagens/excluir.gif';
        btnExcluirEtapa.setAttribute('border', '0');
        
        linkExcluir.appendChild(btnExcluirEtapa);        
        
        var link = document.createElement('a');
        link.setAttribute('href', '#');        
        link.onclick = function()
        {
        	removerEtapa(this.parentNode.parentNode.getAttribute("id"), anxid);
        }        
        
        <?$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($anexo['anxid']/1000) .'/'. $anexo['anxid'];?>
        
        var btnAnexo = document.createElement('img');
        btnAnexo.src = '/imagens/consultar.gif';
        btnAnexo.setAttribute('border', '0');
        
        var link = document.createElement('a');
        link.setAttribute('href', '#');
        link.onclick = function()
        {
        	document.formulario.download.value="1";
        	document.formulario.arquivoid.value=id;
        	document.formulario.submit();
        }        
        link.appendChild(btnAnexo);
        
        var btnpixel = document.createElement('img');
        btnpixel.src = '/imagens/pixel.gif';
        
        // descricao
        input_descricao    = document.createElement('input');
        input_descricao.setAttribute('type', 'text');
        input_descricao.setAttribute('class', 'normal');
        input_descricao.setAttribute('name', 'descricao[]');
        input_descricao.setAttribute('maxlength', '100');
        input_descricao.setAttribute('size', '30');
        input_descricao.setAttribute('value', descricao);
      	        
        //hidden, id da etapa
        input_anxid     = document.createElement('input');
        input_anxid.setAttribute('type', 'hidden');
        input_anxid.setAttribute('name', 'anxid[]');
        input_anxid.setAttribute('value', anxid);

        //combo das etapas
        
        input_arqid   = document.createElement('input');
        input_arqid.setAttribute('class', 'normal');
        input_arqid.setAttribute('name', 'arqid[]');
        input_arqid.setAttribute('type', 'file');
        input_arqid.setAttribute('size', '40');
        input_arqid.setAttribute('value', arqid );

        var tabelaanexosAjax = $('anexosAjax');
        var novaLinha                    = tabelaanexosAjax.insertRow(tabelaanexosAjax.rows.length);
        novaLinha.id                     = 'linhaanexosAjax' + tabelaanexosAjax.rows.length + 1;
        novaLinha.style.textAlign        = 'center'
        novaLinha.style.backgroundColor  = '#f0f0f0';

        /**
         * Botao excluir
         */
        var novaCelula0 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula0.appendChild(linkExcluir);
        novaCelula0.appendChild(btnpixel);
        novaCelula0.appendChild(link);        
        
        /**
         * Botao em branco (separador)
         */
        novaCelula0.appendChild(btnpixel);
                
        /**
         * Botao anexo
         */
        novaCelula0.appendChild(link);
        
        /**
         * Data inicio
         */
        var novaCelula2 = novaLinha.insertCell(novaLinha.cells.length);
        input_descricao.setAttribute('id', novaLinha.cells.length);
        novaCelula2.appendChild(input_descricao);
        novaCelula2.appendChild(input_anxid);

        /**
         * Data fim
         */
        var novaCelula3 = novaLinha.insertCell(novaLinha.cells.length);
      
        novaCelula3.appendChild(input_arqid);

        return false;
    }
</script>
</html>