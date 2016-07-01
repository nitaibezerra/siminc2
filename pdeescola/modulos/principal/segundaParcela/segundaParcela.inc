<?php

if(!$_SESSION['pdeid']){
	
	echo "<script>
			alert('A sessão expirou!');
			history.back(-1);
		  </script>";
	die();	
}

$verificaAutoavaliacao = verificaPreenchimentoAutoavaliacao();

if(!$verificaAutoavaliacao){
	
	echo "<script>
			alert('A autoavaliação deve ser preenchida!');
			history.back(-1);
		  </script>";
	die();	
}

if($_POST['mostraValorRestante'] == 'true'){
	
	$sql = "SELECT				
				sum((pasp.pasquantidade * pasp.pasvalorunitario)) as total
			FROM pdeescola.objetivosegundaparcela osp
			INNER JOIN pdeescola.especificacaosegundaparcela 	esp  ON  esp.ospid = osp.ospid
			INNER JOIN pdeescola.metasegundaparcela 		msp  ON  msp.espid = esp.espid
			INNER JOIN pdeescola.acaosegundaparcela 		p    ON    p.mspid = msp.mspid
			INNER JOIN pdeescola.planoacaosegundaparcela 		pasp ON pasp.aspid = p.aspid
			LEFT JOIN pdeescola.tiporecurso 			tr   ON   tr.treid = pasp.treid
			WHERE pdeid = {$_SESSION['pdeid']}
			AND pasp.treid = {$_REQUEST['treid']}";
			
	$totalValorAcoes = $db->pegaUm($sql);	
			
	$sql = "SELECT
				coalesce(pde.pdevlrplanocusteio,0) as pdevlrplanocusteio,
				coalesce(pde.pdevlrplanocapital,0) as pdevlrplanocapital,
				coalesce(pde.pdevlrcomplementarcusteio,0) as pdevlrcomplementarcusteio,
				coalesce(pde.pdevlrcomplementarcapital,0) as pdevlrcomplementarcapital 
			FROM pdeescola.pdeescola pde 			
			WHERE pde.entid = '{$_SESSION['entid']}'";
			
	$totalValorParcela = $db->pegaLinha($sql);
			
	if($_REQUEST['treid'] == 1){
		$tipo = 'Capital';
		$valorRestante = $totalValorParcela['pdevlrcomplementarcapital'] - $totalValorAcoes;
		$totalValorParcelaTipo = $totalValorParcela['pdevlrcomplementarcapital'];
	} else {
		$tipo = 'Custeio';
		$valorRestante = $totalValorParcela['pdevlrcomplementarcusteio'] - $totalValorAcoes;
		$totalValorParcelaTipo = $totalValorParcela['pdevlrcomplementarcusteio'];
	}
	
	if($_POST['carregar']){
		
		$sql = "SELECT 
					osp.ospid,
					esp.espid,
					msp.mspid,
					p.aspid,
					pasp.pasquantidade,
					pasp.pasdescricaoitem,
					pasp.pasvalorunitario,
					pasp.treid  
			    FROM 
			    	pdeescola.planoacaosegundaparcela as pasp
	            INNER JOIN
	            	pdeescola.acaosegundaparcela as p ON p.aspid = pasp.aspid
	            INNER JOIN
	            	pdeescola.metasegundaparcela as msp ON msp.mspid = p.mspid
	            INNER JOIN 
	            	pdeescola.especificacaosegundaparcela as esp ON esp.espid = msp.espid
                INNER JOIN
                	pdeescola.objetivosegundaparcela as osp ON osp.ospid = esp.ospid
                INNER JOIN
                	pdeescola.tiporecurso as tr ON tr.treid = pasp.treid		            	 		    	
			    WHERE 
			        pasid = '{$_REQUEST['carregar']}' ";
			        
	    $dados = $db->carregar( $sql );	
				
		//$dados = $db->pegaLinha($sql);
		$valorAtual = ($dados['pasquantidade']*$dados['pasvalorunitario']);
		$valorRestante = $valorRestante+$valorAtual;
		
	}

	echo "<b>{$tipo} restante: R$ ".simec_number_format($valorRestante,2,',','.')."</b>";
	echo '<input type="hidden" id="vlrdisponivel" name="vlrdisponivel" value="'.$valorRestante.'">';
	exit();
}

//select para carregar via Ajax - Estratégia
if( $_POST['ajaxospid'] ){	 
	header('content-type: text/html; charset=ISO-8859-1'); 
	$sql = "SELECT
				espid as codigo, 
				espdescricao as descricao 
			FROM
			 	pdeescola.especificacaosegundaparcela 
			WHERE
			 	ospid = '".$_POST['ajaxospid']."' 
			ORDER BY
			 	espdescricao asc"; 
	die($db->monta_combo( "espid", $sql, 'S', 'Selecione...', 'filtraEspecificacao(this.value);', '', '', '', 'S', 'espid' ));
}

//select para carregar via Ajax - Meta
if( $_POST['ajaxespid'] ){	 
	header('content-type: text/html; charset=ISO-8859-1'); 
	$sql = "SELECT
				mspid as codigo, 
				mspdescricao as descricao 
			FROM
			 	pdeescola.metasegundaparcela 
			WHERE
			 	espid = '".$_POST['ajaxespid']."' 
			ORDER BY
			 	mspdescricao asc"; 
	die($db->monta_combo( "mspid", $sql, 'S', 'Selecione...', 'filtraMeta(this.value);', '', '', '', 'S', 'mspid' ));
}

//select para carregar via Ajax - Ação
if( $_POST['ajaxmspid'] ){	 
	header('content-type: text/html; charset=ISO-8859-1'); 
	$sql = "SELECT
				aspid as codigo, 
				aspdescricao as descricao 
			FROM
			 	pdeescola.acaosegundaparcela 
			WHERE
			 	mspid = '".$_POST['ajaxmspid']."'
			AND aspstatus = 'A' 
			ORDER BY
			 	aspdescricao asc"; 
	die($db->monta_combo( "aspid", $sql, 'S', 'Selecione...', '', '', '', '', 'S', 'aspid' ));
}

//***************************************************************************
if (!$_SESSION['entid']){
	die('<script>
			history.go(-1);
		 </script>');
}

//***************************************************************************
$entid             = $_SESSION['entid'];
//$pdeid			   = pegarPdeid($entid);
$docid 			   = pegarDocid($entid);
$_SESSION['pdeid'] = pegarPdeid($entid);
//***************************************************************************


//Permitir que o usuario use a pagina somente se ele estiver preenchido o Resultado da autoavaliação
if($entid){
$sql = "SELECT
			entid,
			qtdap	

from pdeescola.preenchimento
			 WHERE entid ='".$entid."'";

$dados = $db->carregar($sql);
$qtdap = $dados[0]["qtdap"];
//dbg($qtdap,1);
}
	if($qtdap == 0){
		echo("<script>alert('É Necessário Preencher a Autoavaliação.')\n</script>");
		echo("<script>history.go(-1);</script>");
		exit();
}	


//CARREGAR CAMPOS
if (array_key_exists('carregar', $_REQUEST ) ){
		$sql = "SELECT 
					osp.ospid,
					esp.espid,
					msp.mspid,
					p.aspid,
					pasp.pasquantidade,
					pasp.pasdescricaoitem,
					pasp.pasvalorunitario,
					pasp.treid  
			    FROM 
			    	pdeescola.planoacaosegundaparcela as pasp
	            INNER JOIN
	            	pdeescola.acaosegundaparcela as p ON p.aspid = pasp.aspid
	            INNER JOIN
	            	pdeescola.metasegundaparcela as msp ON msp.mspid = p.mspid
	            INNER JOIN 
	            	pdeescola.especificacaosegundaparcela as esp ON esp.espid = msp.espid
                INNER JOIN
                	pdeescola.objetivosegundaparcela as osp ON osp.ospid = esp.ospid
                INNER JOIN
                	pdeescola.tiporecurso as tr ON tr.treid = pasp.treid		            	 		    	
			    WHERE 
			        pasid = '{$_REQUEST['carregar']}' ";
	    $dados = $db->carregar( $sql );	
}	

//Ação = GRAVAR
if ($_REQUEST['evento'] == 'inserir'){
	if( !array_key_exists('carregar', $_REQUEST )){
			//não permitir salvar dados em sessão diferente.
			if(($_POST['pdeid']) == ($_SESSION['pdeid'])){

				if($_POST['aspid'] == '' || $_POST['aspid'] == null){
					echo '<script>
						      alert(\'Não foi possível inserir a ação, tente novamente.\');
						      document.location.href = \'?modulo=principal/segundaParcela/segundaParcela&acao=A\';
					      </script>';
					exit();
				}
				
			$sql = "INSERT INTO 
						pdeescola.planoacaosegundaparcela 
						(pdeid, 
						aspid, 
						pasquantidade, 
						pasdescricaoitem,
						pasvalorunitario, 
						treid) 
					VALUES 
						( ".$_SESSION['pdeid'].", 
						  ".$_POST['aspid'].", 
						  ".$_POST['pasquantidade'].",
						  '".pg_escape_string(substr($_REQUEST["pasdescricaoitem"],0,500))."', 
						  ".str_replace(',','.',(str_replace('.','',$_POST['pasvalorunitario']))).", 
						  ".$_POST['treid'].")";
	
			if ($insert = $db->executar( $sql ) ){
				$db->commit( $sql );
				echo("<script>alert('Operação realizada com sucesso.')\n</script>");
				echo("<script>window.location.href = 'pdeescola.php?modulo=principal/segundaParcela/segundaParcela&acao=A'</script>");				
			}
		}else{
			echo("<script>alert('Não é permitido acessar duas escolas concorrentemente. Tente novamente!')\n</script>");
			echo("<script>window.location.href = 'pdeescola.php?modulo=lista&acao=E'</script>");
		}
	//Ação = ALTERAR
	}elseif(array_key_exists('carregar', $_REQUEST )){
		$sql = sprintf("SELECT 
							pasid 
						FROM 
							pdeescola.planoacaosegundaparcela
		 				WHERE
		 					pasid = '%s'",
						$_REQUEST['carregar']
					);

		$planoAlteracao = $db->carregar($sql);
		$planoAlteracao = $planoAlteracao ? $planoAlteracao : array();
			
			//****************************************************************************
			
			if (count($planoAlteracao)){
				//não permitir salvar dados em sessão diferente.
				if(($_POST['pdeid']) == ($_SESSION['pdeid'])){
					$sql = "UPDATE pdeescola.planoacaosegundaparcela
							SET 
								aspid 				= ".$_REQUEST["aspid"].",
								pasquantidade 		= ".$_REQUEST["pasquantidade"].",
								pasdescricaoitem    = '".pg_escape_string(substr($_REQUEST["pasdescricaoitem"],0,500))."', 
								pasvalorunitario 	= '".str_replace(',','.',str_replace('.','',(substr($_REQUEST["pasvalorunitario"],0,10))))."',
								treid 				= ".$_REQUEST["treid"]."						
							WHERE
								pdeid = ". $_SESSION['pdeid']."
							AND
								pasid = ".$_REQUEST['carregar'];
	
					if ( $db->executar( $sql ) ){
						$db->commit( $sql );
						echo("<script>alert('Operação realizada com sucesso.')\n</script>");
						echo("<script>window.location.href = 'pdeescola.php?modulo=principal/segundaParcela/segundaParcela&acao=A'</script>");	
						
					}else{
						echo("<script>alert('Não foi possível a alteração dos dados.')\n</script>");
					}
				}else{
					echo("<script>alert('Não é permitido acessar duas escolas concorrentemente. Tente novamente!.')\n</script>");
					echo("<script>window.location.href = 'pdeescola.php?modulo=lista&acao=E'</script>");
				}
		
			unset($_POST['pasid']);
	}
}
}
//Ação = EXCLUIR
if (array_key_exists('excluir', $_REQUEST ) ){
	$sql = "DELETE FROM pdeescola.planoacaosegundaparcela WHERE pasid = '{$_REQUEST['excluir']}'";

	$db->executar( $sql );
	$db->commit();
	echo("<script>window.location.href = 'pdeescola.php?modulo=principal/segundaParcela/segundaParcela&acao=A'</script>");
}


include  APPRAIZ."includes/cabecalho.inc";
echo "<br />";
echo montarAbasArray(carregaAbas(), "/pdeescola/pdeescola.php?modulo=principal/segundaParcela/segundaParcela&acao=A");
$db->cria_aba($abacod_tela,$url,$parametros);

$titulo_modulo='Parcela Complementar';
monta_titulo($titulo_modulo,'');

echo cabecalhoPDE();
?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script>
	function mostraValorRestante(treid, carregar){
	
		if(treid){
		
			if(carregar){
				paramentros = 'mostraValorRestante=true&treid='+treid+'&carregar='+carregar;
			}else{
				paramentros = 'mostraValorRestante=true&treid='+treid;
			}
		
			new Ajax.Request('pdeescola.php?modulo=principal/segundaParcela/segundaParcela&acao=A',{
			
				method: 'post',
				parameters: paramentros, 
				onComplete: function(res){
					
					$('divValoresRestantes').style.display = "block";
					$('divValoresRestantes').innerHTML = res.responseText;
				}
			});
		}
	
	}
	
	function verificaValorPeloRestante(valor){
		
		pasquantidade = $('pasquantidade').value;
		
		if(pasquantidade == '' || pasquantidade == 0){
		
			alert("Digite um valor para a quantidade!");
			$('pasquantidade').focus();
			return false;
		}				
		
		if($('vlrdisponivel')){
		
			if($('vlrdisponivel') == ''){
		
				alert("Escolha uma natureza da despeza!");
				$('treid').focus();
				return false;
			}

			//valor que falta para declarar
			vlrdisponivel = new Number($('vlrdisponivel').value);

			//valor armazenado no banco
			vlrbase = new Number($('valorbanco').value);
			
			//valor total que o sistema espera receber (valor armazenado no campo + o que falta)
			vlraceito = new Number(vlrbase + vlrdisponivel);

			//valor informado no campo (atual)
			valor = valor.value.replace(".","").replace(",",".");
			valor = parseInt(pasquantidade)*parseFloat(valor);

			if(valor > vlraceito){
			
				alert("O valor total informado é maior que o valor disponível!");
				return false;
				
			} else {
			
				return true;
			}
		}
		
	}
</script>
          
 <form name="formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario">
 	<input type="hidden" name="evento">
 	<input type="hidden" name="pdeid" value="<?php echo $_SESSION['pdeid']?>">
 	<input type="hidden" name="valorbanco" id="valorbanco" value="<?php echo $dados[0]['pasvalorunitario']?>">
 	<div style="position:absolute;float:right;right:5%;padding:5px;">
 	<?php 
 	include_once APPRAIZ . 'includes/workflow.php';
 	wf_desenhaBarraNavegacao( $docid , array( 'entid' => $_SESSION['entid'] ) );
 	?>
 	</div> 	
     <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-top:0px;">
       <tr>
         <td class ="SubTituloDireita" align="right" width="20%">Objetivo: </td>
         <td id="td_objetivo">
         <? $ospid = $dados[0]['ospid']; 
            $sql = "SELECT 
                      ospid  AS codigo, 
                      ospdescricao AS descricao
                    FROM
                      pdeescola.objetivosegundaparcela
                    WHERE
                      ospstatus = 'A'
                    ORDER BY
                      ospitem"; 
                 
            $db->monta_combo('ospid', $sql, 'S', "Selecione...", 'filtraObjetivo(this.value);', '', '', '', 'S', 'ospid','',$ospid);
         ?>
         </td>
                <tr>
         <td class ="SubTituloDireita">Estratégia: </td>
         <td id="td_especificacao">
         <? $espid = $dados[0]['espid']; 

         	if($ospid){
         		$sql = "SELECT
							espid as codigo, 
							espdescricao as descricao 
						FROM
						 	pdeescola.especificacaosegundaparcela 
						WHERE
						 	ospid = '".$ospid."' 
						ORDER BY
						 	espdescricao asc";
         		$db->monta_combo('espid', $sql, 'S', "Selecione...", 'filtraEspecificacao(this.value)', '', '', '', 'S', 'espid','',$espid);
         	}else{
          		$sql = array();
            	$db->monta_combo('espid', $sql, 'N', "Selecione...", '', '', '', '', 'S', 'espid');
         	}
         ?>
         </td>
       </tr> 
       <tr>
         <td class ="SubTituloDireita">Meta: </td>
         <td id="td_meta">
         <? $mspid = $dados[0]['mspid'];
            
            if($espid){
	            $sql = "SELECT
							mspid as codigo, 
							mspdescricao as descricao 
						FROM
						 	pdeescola.metasegundaparcela 
						WHERE
						 	espid = '".$espid."' 
						ORDER BY
						 	mspdescricao asc";
            $db->monta_combo('mspid', $sql, 'S', "Selecione...", 'filtraMeta(this.value)', '', '', '', 'S', 'mspid','',$mspid);
            }else{
	            $sql = array();
            $db->monta_combo('mspid', $sql, 'N', "Selecione...", '', '', '', '', 'S', 'mspid');
            }
         ?>
         </td>
       </tr>
       <tr>
         <td class ="SubTituloDireita">Ação: </td>
         <td id="td_parcela">
         <? $aspid = $dados[0]['aspid'];
			
            if($mspid){
	         	$sql = "SELECT
							aspid as codigo, 
							aspdescricao as descricao 
						FROM
						 	pdeescola.acaosegundaparcela 
						WHERE
						 	mspid = '".$mspid."'		
						AND aspstatus = 'A'				
						ORDER BY
						 	aspdescricao asc";	         	
	         	$bloq = 'S';
            }else{
	            $sql = array();
	            $bloq = 'N';
            }
            $db->monta_combo('aspid', $sql, $bloq, "Selecione...", '', '', '', '', 'S', 'aspid');
         ?>
         </td>
       </tr> 
       <tr>
	   	 <td class="SubTituloDireita">Descrição Item:</td>
		 <td id="td_descricao">
		 <?
		    $pasdescricaoitem = $dados[0]['pasdescricaoitem'];
			echo campo_textarea('pasdescricaoitem',true,'S','', 100, 5, 500,'id="pasdescricaoitem"');
		 ?>
		 </td>
	   </tr>
	   <tr>
         <td class ="SubTituloDireita">Natureza de despesa: </td>
         <td id="td_natureza">
         <? $treid = $dados[0]['treid']; 
            $sql = "SELECT 
                        treid AS codigo, 
                        tredescricao AS descricao
                    FROM
                        pdeescola.tiporecurso
					ORDER BY 
						tredescricao"; 
            
            $db->monta_combo('treid', $sql, 'S', "Selecione...", 'mostraValorRestante', '', '', '145', 'S', 'treid');
         ?>
         </td>
       </tr> 
       <tr>
         <td class ="SubTituloDireita">Quantidade: </td>
		 <td id="td_quantidade">
		 <? 
		 	$pasquantidade = $dados[0]['pasquantidade'];
		 	//echo campo_texto( 'pasquantidade', 'S', 'S', '', 20, 50, '', ''); 
		 	echo campo_texto('pasquantidade', 'S', 'S' , '', 25, 50, '##########', '', 'left', '', 0, 'id="pasquantidade"')
		 ?>
         </td>
       </tr>
       <tr>
         <td class ="SubTituloDireita" align="right" width="20%">Valor Unitário: </td>
		 <td id="td_valor">
		 <? //$complemento = ' id="pasvalorunitario" onkeypress="alterar_campo_focado( event, \'valor\' );" ';
			//echo campo_texto( 'pasvalorunitario', 'N', 'S', 'Valor', 25, 14, '##.###.###.###,##', '', 'right', '', 0, $complemento );
			$pasvalorunitario = simec_number_format(str_replace(',','',$dados[0]['pasvalorunitario']),2,',','.');
			echo campo_texto( 'pasvalorunitario', 'N', 'S', 'Valor', 25, 50, '', '', 'left', '', 0, 'id="obrcustocontrato" onkeyup="this.value=mascaraglobal(\'##.###.###,##\',this.value);" onfocus="this.select();"');
			
			$sql = "SELECT
						coalesce(pde.pdevlrplanocusteio,0) as pdevlrplanocusteio,
						coalesce(pde.pdevlrplanocapital,0) as pdevlrplanocapital,
						coalesce(pde.pdevlrcomplementarcusteio,0) as pdevlrcomplementarcusteio,
						coalesce(pde.pdevlrcomplementarcapital,0) as pdevlrcomplementarcapital 
					FROM pdeescola.pdeescola pde 
					WHERE pde.entid = '{$_SESSION['entid']}'";			
			
			//echo "<br />";
			//echo "Restante Custeio: R$<br>";
			//echo "Restante Capital: R$";
		 ?>
		 <div id="divValoresRestantes" style="display:none;color:red;padding:5;"></div>		 
         </td>
       </tr>
       <?php 
       $perfil = arrayPerfil();      
       
       if(in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_MUNICIPAL, $perfil) ||
       	  in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_ESTADUAL, $perfil) ||
       	  in_array(PDEESC_PERFIL_SUPER_USUARIO, $perfil)){ 
       ?>       
	   <tr>
	   	 <td colspan="2" class ="SubTituloDireita">
	   	 	<center>
	   	 		<?php
	   	 		
	   	 		$verificaParcela = verificaParcelaComplementar();
	   	 		$esdid = pegarEstadoAtual($docid);
	   	 		
//	   	 		$arEstadosWorkflow = array(VALIDACAO_PELO_MEC_WF, AVALIACAO_COMITE_ME_WF, DEVOLVIDO_PARA_ESCOLA_PC_WF);
				$arEstadosWorkflow = array(VALIDACAO_PELO_MEC_WF, DEVOLVIDO_PARA_ESCOLA_PC_WF);
	   	 		
	   	 		if(in_array($esdid, $arEstadosWorkflow)){

	   	 		?>
	   	 		<input type="button" name="btnGravar" value="Gravar" id="btnGravar" onclick="validaForm('grava');">
	   	 		<?php } ?>
	   	 		<?php if($_GET['carregar']){ ?>
	   	 		<input type="button" name="btnNovo" value="Novo" id="btnNovo" onclick="javascrip:document.location.href = '?modulo=principal/segundaParcela/segundaParcela&acao=A';">
	   	 		<?php } ?>
	   	 	</center>
	   	 </td>
       </tr> 
       <?php } ?>
     </table>
 </form>
 <br>
 <br>
 <?php 
 $sql = $db->pegaUm("SELECT pasid FROM pdeescola.planoacaosegundaparcela WHERE pdeid = ".$_SESSION['pdeid']);
 if($sql){?>
 <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
		<td style="padding:15px; background-color:#fafafa; color:#404040; vertical-align: top;" colspan="4">
			<div class="dtree" style="overflow: hidden;">
 				<p>
 					<a style="font-size: 11px; color: #333333" href="pdeescola.php?modulo=principal/segundaParcela/consultaPlanoAcaoSegundaParcela&acao=A">Visualizar Todos os Planos</a>
 				</p>
 			</div>
 		</td>
 	</tr>
 </table>
<?php }?>
    <?php
    
    	      
       
		if(in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_MUNICIPAL, $perfil) ||
       	   in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_ESTADUAL, $perfil) ){
       	   		if(in_array($esdid, $arEstadosWorkflow)){
    				$btExcluir = "<img onclick=\"return excluirRegistro('|| pasp.pasid ||')\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir Registro\" />";
    			}
		} else {
			
			$btExcluir = "";
		}
        //gera lista para monta-lista       
        $sql = "SELECT 
					'<img onclick=\"return alterarRegistro('|| pasp.pasid ||',' || pasp.treid || ')\" src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar Registro\" />
					{$btExcluir}' as acoes,				    
					('<strong>Objetivo:</strong> ' || osp.ospdescricao || '<br>'|| '<strong>Estratégia:</strong> ' || esp.espdescricao) as obj_est,
					msp.mspdescricao,
					p.aspdescricao,					
					'<center>' || pasp.pasquantidade || '</center>' as pasquantidade,
					pasp.pasdescricaoitem,
					to_char(pasvalorunitario, '999g999g999g990d99'),
					tr.tredescricao,
					(pasp.pasquantidade * pasp.pasvalorunitario)
				FROM
					pdeescola.objetivosegundaparcela osp
				INNER JOIN
					pdeescola.especificacaosegundaparcela esp ON esp.ospid = osp.ospid
				INNER JOIN
					pdeescola.metasegundaparcela msp ON msp.espid = esp.espid
				INNER JOIN
					pdeescola.acaosegundaparcela p ON p.mspid = msp.mspid
				INNER JOIN 
					pdeescola.planoacaosegundaparcela pasp ON pasp.aspid = p.aspid
				LEFT JOIN 
					pdeescola.tiporecurso tr ON tr.treid = pasp.treid
				WHERE
					pdeid = ".$_SESSION['pdeid'];
            
        $cabecalho = array("&nbsp;&nbsp;&nbsp;&nbsp;Ação", "Objetivo/Estratégia", "Meta", "Ação", "Quantidade", "Descrição", "Valor Unitário", "Natureza Despesa", "Total" ); 
	    $db->monta_lista( $sql, $cabecalho, 25, 10, 'S', '', 'S');

	    if($_GET['carregar']){
			
			echo '<script>mostraValorRestante('.$_GET['tipo'].', '.$_GET['carregar'].');</script>';
		}
	                              
    ?>

 <script type="text/javascript">

 function validaForm(type){  

		 
	var type;
	if( type == 'grava' ){

	var ospid = document.formulario.ospid.value;
	if( ospid == '' ){
		alert('É necessário informar o Objetivo.');
		return false;
	}
	var espid = document.formulario.espid.value;
	if( espid == '' ){
		alert('É necessário informar a Estratégia.');
		return false;
	}
	var mspid = document.formulario.mspid.value;
	if( mspid == '' ){
		alert('É necessário informar a Meta.');
		return false;
	}
	var aspid = document.formulario.aspid.value;
	if( aspid == '' ){
		alert('É necessário informar a Ação.');
		return false;
	}
	var pasdescricaoitem = document.formulario.pasdescricaoitem.value;
	if(pasdescricaoitem == ''){
		alert('É necessário informar Descrição Item.');
		return false;
	}
	var pasquantidade = document.formulario.pasquantidade.value;
	if( pasquantidade == '' ){
		alert('É necessário informar a Quantidade.');
		return false;
	}
	var pasvalorunitario = document.formulario.pasvalorunitario.value;
	if( pasvalorunitario == '' ){
		alert('É necessário informar o Valor Unitário.');
		return false;
	}
	var treid = document.formulario.treid.value;
	if( treid == '' ){
		alert('É necessário informar o Natureza de Despesa.');
		return false;
	}
	document.formulario.evento.value = "inserir";
}

if(!verificaValorPeloRestante($('obrcustocontrato'))){
	return false;
}

document.formulario.submit(); 

}
 
function excluirRegistro(pasid){
    if (!confirm('Deseja excluir definitivamente o registro?')) {
    	return false;
    }
     return window.location.href = 'pdeescola.php?modulo=principal/segundaParcela/segundaParcela&acao=A&excluir=' + pasid;
       
    
}
         
function alterarRegistro(pasid, treid){    
    return window.location.href = 'pdeescola.php?modulo=principal/segundaParcela/segundaParcela&acao=A&carregar=' + pasid + '&tipo=' + treid;
 }
 
 
function filtraObjetivo(ospid){

	var espid = document.getElementById('espid');
	espid.options[0].selected = true;
	espid.disabled			  = true;
	
	var mspid = document.getElementById('mspid');
	mspid.options[0].selected = true;	
	mspid.disabled 			  = true;

	var aspid = document.getElementById('aspid');
	aspid.options[0].selected = true;	
	aspid.disabled 			  = true;
	
 	if( !ospid ){
 		return false;
 	}
	var td_especificacao = document.getElementById('td_especificacao');
  	 
	var req = new Ajax.Request('pdeescola.php?modulo=principal/segundaParcela/segundaParcela&acao=A', {
							        method:     'post',
							        parameters: '&ajaxospid=' + ospid,							         
							        onComplete: function (res){			
										td_especificacao.innerHTML = res.responseText;
							        }							        
							  });   
}

function filtraEspecificacao(espid){
	
	var mspid = document.getElementById('mspid');
	mspid.options[0].selected = true;	
	mspid.disabled 			  = true;

	var aspid = document.getElementById('aspid');
	aspid.options[0].selected = true;	
	aspid.disabled 			  = true;
	
	if( !espid ){
		return false;
	}
	
	var td_meta = document.getElementById('td_meta');

	var req = new Ajax.Request('pdeescola.php?modulo=principal/segundaParcela/segundaParcela&acao=A', {
        method:     'post',
        parameters: '&ajaxespid=' + espid,							         
        onComplete: function (res){			
			td_meta.innerHTML = res.responseText;
        }       
  });	 
}

function filtraMeta(mspid){
	
	var aspid = document.getElementById('aspid');
	aspid.options[0].selected = true;	
	aspid.disabled 			  = true;
	
	if( !mspid ){
 		return false;
 	}
	var td_parcela = document.getElementById('td_parcela');
  
	var req = new Ajax.Request('pdeescola.php?modulo=principal/segundaParcela/segundaParcela&acao=A', {
							        method:     'post',
							        parameters: '&ajaxmspid=' + mspid,							         
							        onComplete: function (res)
							        {			 
										td_parcela.innerHTML = res.responseText;
							        }							        
							  });   
}

//function alterar_campo_focado(event, campo)
//{
//	if (event.keyCode != 13)
//	{
//		return;
//	}
//
//	switch (campo)
//	{
//		case 'pasvalorunitario':
//			var campo = document.getElementById('pasvalorunitario');
//			break;
//		default:
//			return;
//	}
//
//	campo.focus();
//	campo.select();
//}
//
//var http=createRequestObject();
//var uploader="";
//var uploadDir="";
//var dirname="";
//var filename="";
//var timeInterval="";
//var idname="";
//var uploaderId="";
// 
//function createRequestObject() {
//    var obj;
//    var browser = navigator.appName;
//    if(browser == "Microsoft Internet Explorer"){
//    	document.getElementById('div_files').style.left = "-446px";
//    	document.getElementById('div_files').style.top = "60px";
//    	document.getElementById('imagem').style.left = "0px";
//    	document.getElementById('imagem').style.top = "0px";
//    	return new ActiveXObject("Microsoft.XMLHTTP");
//    }
//    else{
//    	return new XMLHttpRequest();
//    }   
//}
</script>