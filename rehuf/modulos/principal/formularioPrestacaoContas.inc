<?php
include_once APPRAIZ . "includes/classes/Modelo.class.inc";

$_SESSION['rehuf_var']['entid'] = $_GET['entid'] ? $_GET['entid'] : $_SESSION['rehuf_var']['entid'];
$entid = $_SESSION['rehuf_var']['entid'] ? $_SESSION['rehuf_var']['entid'] : $_GET['entid'];

include_once APPRAIZ . "includes/classes/modelo/entidade/Entidade.class.inc";
$e = new Entidade();

if($_GET['entungcod']){
	$entungcod = $_GET['entungcod'];
	
	$entid = $e->buscarentidadeporentungcod($entungcod);
}

if($_GET['prtid']){
	$_SESSION['rehuf_var']['prtid'] = $_GET['prtid'] ? $_GET['prtid'] : $_SESSION['rehuf_var']['prtid'];
	$prtid = $_SESSION['rehuf_var']['prtid'] ? $_SESSION['rehuf_var']['prtid'] : $_GET['prtid'];
}else if($_GET['pltid']){
	$_SESSION['rehuf_var']['pltid'] = $_GET['pltid'] ? $_GET['pltid'] : $_SESSION['rehuf_var']['pltid'];
	$pltid = $_SESSION['rehuf_var']['pltid'] ? $_SESSION['rehuf_var']['pltid'] : $_GET['pltid'];
}

if($_POST['requisicao'])
{
	$arrDados = $_POST['requisicao']();
}

if($_GET['arquivo'])
{
	include_once APPRAIZ . "includes/classes/file.class.inc";
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec();
	$file-> getDownloadArquivo($_GET['arquivo']);
	exit;
}
include_once APPRAIZ . 'includes/workflow.php';
include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';

/* montando o menu */
echo montarAbasArray(carregardadosmenurehuf(), "/rehuf/rehuf.php?modulo=principal/prestacaocontas&acao=A");

monta_titulo( "HU", "Prestação de Contas");
monta_cabecalho_rehuf($entid);
if($entid){
		$e->carregarPorId( $entid );
		extract($e->getDados());
		$arrPortariasentidade = $e->buscarUniversidadeByHospital($entid);
}
include_once APPRAIZ . "rehuf/classes/VinculoPlanoInterno.class.inc";
$prt = new VinculoPlanoInterno();
$vpiid = $prt->buscarVinculoPlanoInterno($prtid,$pltid);

include_once APPRAIZ . "rehuf/classes/PrestacaoContas.class.inc";
$prc = new PrestacaoContas();
$prcug = $prc->buscarPrestacaoContasUg($entungcod,$vpiid);

if(!$prcug){
	$docid = criarDocumentoPrestacaoContas( $prcid );
	$_POST['docid'] = $docid; 
	$_POST['prcug'] = $entungcod;
	$_POST['vpiid'] = $prt->buscarVinculoPlanoInterno($prtid,$pltid);
	$prc->popularDadosObjeto( $_POST );
	$prcid = $prc->salvar();
	$prc->commit();	
}


if($prtid || $pltid){
	extract($prt->buscarplanointernoByPortariaORPlanoTrabalho($entungcod,$prtid,$pltid));
}

if($prcid){
	$prc = new PrestacaoContas();
	$arrRespostasQuestionario = $prc->buscarRespostaQuestionarioPorPrestacaoContas( $prcid );
}
if($vpiid){
	$arrVpqid = $prt->buscarQuestionarioPlanoInterno($vpiid);	
}
$cabecalho = $entungcod. "-" .$arrPortariasentidade['ifes']. "-" .$arrPortariasentidade['entnome'];

include_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . 'includes/classes_simec.inc';
/*
 * Recupera o "docid"
 */

$dataAtualHora = date("Y-m-d H:i:s");


// Parâmetros para a nova conexão com o banco do SIAFI
$servidor_bd = "";
$porta_bd    = "5432";
$nome_bd     = "";
$usuario_db  = "";
$senha_bd    = "";

$db2 = new cls_banco();

// Parâmetros da nova conexão com o banco do SIAFI para o componente 'combo_popup'.
$dados_conexao = array(
					'servidor_bd' => $servidor_bd,
					'porta_bd' => $porta_bd,
					'nome_bd' => $nome_bd,
					'usuario_db' => $usuario_db,
					'senha_bd' => $senha_bd
				);

$arrCabecalho = array('Nº de Empenho','Natureza de Despesa Detalhada','CNPJ do Favorecido','Nome do Favorecido','Empenhos Emitidos R$','Empenhos Liquidados R$','Valores Pagos R$','Exerc.inscr.em RP n-proc');

if($docid){
	//Pegar o documento
	$esdid = pegarEstadoPrestacaoContas($docid);
	//verifica se o estado é diferente de Em elaboração para obrigar a responder o questionário
	if(!in_array($esdid, array(ESTADO_EM_ELABORACAO))){
		$habil = "S";
		$ob = "S";
	}
	 if(in_array($esdid, array(ESTADO_ENCERRADO))){
		$habil = "S";
		$ob = "N";
	}else{
		$ob = "S";
	}
	
	if(in_array($esdid, array(ESTADO_EM_ELABORACAO)) || in_array($esdid, array(ESTADO_EM_AJUSTE))){
		$habilitaBotãoSalvar = "S";
	}
	
//Verifica se o documento já foi desbloqueado
	if($esdid == ESTADO_BLOQUEADO){
		if(verificarEstadoBloqueado($docid)){
			$bloqueado = "N";
			$bloquear = "N";
		}else{
			$bloquear = "N";
		}
	}
	
	$dataInicio = date("d/m/Y", strtotime($plidtinicio)); 
	$dataFinal = date("d/m/Y", strtotime($plidttermino));
	
if($bloquear == "N"){
		if($esdid != ESTADO_ENCERRADO || $esdid != ESTADO_BLOQUEADO){
			$data=date("Ymd");
			//Não bloqueia se a data estiver dentro do período
			if((int)$data <= (int)str_replace("-","",$plidttermino)){
				$bloqueado = 'N';
			}else{ // Se a data estiver fora do período, bloqueia
			    $bloqueado = "S";
			    $habilitaBotãoSalvar = "N";
			    //Pega a ação para boquear (estado atual + estado de bloqueado)
			    if($esdid != ESTADO_BLOQUEADO && !$_SESSION['rehuf']['workflow_miau_'.$docid]){
		    		$aedid = buscaracaoestadodoc($esdid,ESTADO_BLOQUEADO);
		    		if($aedid){
						//Altera o workflow
			    		wf_alterarEstado($docid,$aedid,'',array());
			    		$esdid = pegarEstadoPrestacaoContas($docid);
			    		echo "<script>window.location=window.location</script>";
			    		die;
		    		}	
			    }
			}
		}
	}
}

	if($esdid == ESTADO_BLOQUEADO && ((int)$data >= (int)str_replace("-","",$plidtinicio)) && (int) $data >= (int)str_replace("-","",$plidttermino)){
		$mensagem = "<span style='color:#FF0000;font-weight:bold'>O seu formulário está bloqueado. O período para prestação de contas era  $dataInicio a $dataFinal. Por favor contactar a DAF.</span>";
	}
/*
 * Acrescentado a pedido da Eliane, para quando o usuário informar uma data de início superior a data atual ele bloquear e só desbloquear quando chegar a data.
 * Desbloquear caso esteja dentro do período
 */
//verifica se está no período
//	if((int)$data >= (int)str_replace("-","",$plidtinicio) && (int)$data <= (int)str_replace("-","",$plidttermino)){
//		//Verifica se o estado é bloqueado
//		if($esdid == ESTADO_BLOQUEADO){
//		$aedid = buscaracaoestadodoc($esdid,WF_ACAO_DESBLOQUEADO);
//				//Altera o workflow
//	    		wf_alterarEstado($docid,$aedid,'',array());
//	    		$esdid = pegarEstadoPrestacaoContas($docid);
//	    		echo "<script>window.location=window.location</script>";
//	    		die;	
//		}
//	}else if(((int)$data <= (int)str_replace("-","",$plidtinicio))){
//		//Verifica se o estado está bloqueado e se a data inicial é maior que a data atual para apenas mostrar a mensagem.
//		if($esdid == ESTADO_BLOQUEADO){
//			if(((int)$data <= (int)str_replace("-","",$plidtinicio)) && (int)$data <= (int)str_replace("-","",$plidttermino)){
//				$mensagem = "<span style='color:#FF0000;font-weight:bold'>O seu formulário está bloqueado. O período para prestação de contas será de  $dataInicio a $dataFinal.</span>";
//				//Prazo ultrapassado
//			}
//		}
//		//Verifica se o estado é bloqueado
//		if($esdid != ESTADO_BLOQUEADO && (int)$data <= (int)str_replace("-","",$plidttermino)){
//			$aedid = buscaracaoestadodoc($esdid,ESTADO_BLOQUEADO);
//				//Altera o workflow
//	    		wf_alterarEstado($docid,$aedid,'',array());
//	    		$esdid = pegarEstadoPrestacaoContas($docid);
//	    		echo "<script>window.location=window.location</script>";
//	    		die;	
//		}
//}

?>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>

jQuery(function() {
	<?php if($_SESSION['rehuf']['altert']): ?>
		alert('<?php echo $_SESSION['rehuf']['altert'] ?>');
		<?php unset($_SESSION['rehuf']['altert']) ?>
	<?php endif; ?>
});
			
function voltar(){
	<?php 
		unset($_SESSION['rehuf_var']['prtid']);
		unset($_SESSION['rehuf_var']['pltid']);
	 ?>
	var url = "rehuf.php?modulo=principal/prestacaocontas&acao=A";
		jQuery(location).attr('href',url);
	}
function salvarPrestacaoContas(){
			var erro = 0;
			jQuery("[class~=obrigatorio]").each(function() { 
				if(!this.value || this.value == "Selecione..."){
					erro = 1;
					alert('Favor preencher todos os campos obrigatórios!');
					this.focus();
					return false;
				}
			});
			if(erro == 0){
				jQuery("#formulario").submit();
			}
}

function geraPDF(){
			
			var vpiid = jQuery("#vpiid").val();
			var entid = jQuery("#entid").val();
			var prtid = jQuery("#prtid").val();
			var pltid = jQuery("#pltid").val();
				var url = "rehuf.php?modulo=relatorio/gerarPdfPrestacaoContas&acao=A&entid="+entid+"&vpiid="+vpiid+"&prtid="+prtid+"&pltid="+pltid;
  				//jQuery(location).attr('href',url);
  				window.open(url);
	//form.submit();

}
function abrirTutorial(){
	
}
</script>

<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>
	<tr>
		<td align="center">
			<?php 
				if($esdid == ESTADO_BLOQUEADO){
					echo $mensagem;
				}
			?>
		</td>
	<tr>
	<tr id="tr_nomehospital">
		<td class="SubTituloCentro"><?=$cabecalho  ?></td>
	</tr>
		<tr id="tr_nomehospital">
			<td class="SubTituloCentro">Identificação</td>
	</tr>
</table>

<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>
	<tr>
		<td class="SubTituloCentro">
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<?php if(!$plttitulo): ?>
						<tr>
							<td width="32%" class="SubTituloDireita">Portaria:</td>
							<td>
								<?= "Portaria Nº {$prtnumero}, de ".exibeDataPorExtenso($prtdata)." - DOU {$prtnumerodou}, de ".formata_data($prtdtpublicacao)."." ?>
							</td>
						</tr>
					<?php else: ?>
						<tr>
							<td width="25%" class="SubTituloDireita">Plano Trabalho:</td>
							<td>
								<?=$plttitulo; ?>
							</td>
						</tr>
					<?php endif; ?>
					<tr>
						<td width="25%" class="SubTituloDireita">Código Unidade Gestora:</td>
						<td>
							<?=$entungcod ?>
						</td>
					</tr>
					<tr>
						<td width="25%" class="SubTituloDireita">IFES:</td>
						<td>
							<?=$arrPortariasentidade['ifes']?>
						</td>
					</tr>
					<tr>
						<td width="25%" class="SubTituloDireita">Plano Interno:</td>
						<td>
							<?=mb_strtoupper($plinumero) ?>
						</td>
					</tr>
					<tr>
						<td width="25%" class="SubTituloDireita">Arquivo:</td>
						<td>
							<?php
								if($arqid){
									if($prtid){
										echo "<a href='rehuf.php?modulo=principal/formularioPrestacaoContas&acao=A&prtid={$prtid}&arquivo=".$arqid."' ><img border=0 src=\"../imagens/anexo.gif\" title=\"{$arqnome}\" /> {$arqnome}</a> ";
									}else{
										echo "<a href='rehuf.php?modulo=principal/formularioPrestacaoContas&acao=A&pltid={$pltid}&arquivo=".$arqid."' ><img border=0 src=\"../imagens/anexo.gif\" title=\"{$arqnome}\" /> {$arqnome}</a> ";
									}
								}else{
									echo "N/A";
								} 
							?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloCentro" colspan="14">Empenhos Custeio</td>
					</tr>
					<tr>
						<td class="SubTituloCentro" colspan="14" id="empenho_custeio">
							<?php 
								$plinumero = mb_strtoupper($plinumero);
								$sql1 = "SELECT
						 						conta_corrente AS cod_agrupador1,'<span></span>' || naturezadet AS cod_agrupador2, codigo_favorecido, it_no_credor,   
										       --'' AS dsc_agrupador1,naturezadet_desc AS dsc_agrupador2,
										       sum(valor1) AS coluna1,sum(valor2) AS coluna2,sum(valor3) AS coluna3,sum(valor4) AS coluna4
										FROM
										(SELECT 
										   sld.sldcontacorrente AS conta_corrente,sld.ctecod || '.' || sld.gndcod || '.' || sld.mapcod || '.' || sld.edpcod || '.' || substr(trim(sld.sldcontacorrente), 13, 2)::character varying(2) AS naturezadet,
										   sld.sldcontacorrente AS conta_corrente_desc,ndp.ndpdsc AS naturezadet_desc,
										   CASE WHEN sld.sldcontacontabil in ('292410101', '292410402', '292410403', '292410405') THEN 
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.0435 ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor1,CASE WHEN sld.sldcontacontabil in ('292410402','292410403') THEN 
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.0435 ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor2,CASE WHEN sld.sldcontacontabil in ('292410403') THEN 
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.0435 ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor3,CASE WHEN sld.sldcontacontabil in ('292410405') THEN 
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.0435 ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor4
									       FROM
										   dw.saldo2012 sld
										   LEFT JOIN ( select distinct ctecod, gndcod, mapcod, edpcod, sbecod, ndpdsc from dw.naturezadespesa where sbecod <> '00' ) as ndp ON cast(ndp.ctecod AS text) = sld.ctecod AND cast(ndp.gndcod AS text) = sld.gndcod AND cast(ndp.mapcod AS text) = sld.mapcod AND cast(ndp.edpcod AS text) = sld.edpcod AND cast(ndp.sbecod AS text) = sld.sbecod
										   WHERE sld.ungcod in ('{$entungcod}') AND sld.plicod = '{$plinumero}' AND sld.sldcontacontabil in ( '292410101', '292410402', '292410403', '292410405', '292410402','292410403', '292410403', '292410405' )
										    UNION ALL	
										   SELECT 
										   sld.sldcontacorrente AS conta_corrente,null,
										   sld.sldcontacorrente AS conta_corrente_desc,null,
										   CASE WHEN sld.sldcontacontabil in ('292410101', '292410402', '292410403', '292410405') THEN 
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.0435 ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor1,CASE WHEN sld.sldcontacontabil in ('292410402','292410403') THEN 
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.0435 ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor2,CASE WHEN sld.sldcontacontabil in ('292410403') THEN 
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.0435 ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor3,CASE WHEN sld.sldcontacontabil in ('292410405') THEN 
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.0435 ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor4 
									       FROM 
										   dw.saldo2012 sld
										   LEFT JOIN ( select distinct ctecod, gndcod, mapcod, edpcod, sbecod, ndpdsc from dw.naturezadespesa where sbecod <> '00' ) as ndp ON cast(ndp.ctecod AS text) = sld.ctecod AND cast(ndp.gndcod AS text) = sld.gndcod AND cast(ndp.mapcod AS text) = sld.mapcod AND cast(ndp.edpcod AS text) = sld.edpcod AND cast(ndp.sbecod AS text) = sld.sbecod
										   WHERE sld.ungcod in ('{$entungcod}') AND sld.plicod = '{$plinumero}' AND sld.sldcontacontabil in ('292410101', '292410402', '292410403', '292410405', '292410402','292410403', '292410403', '292410405')
									   ) as foo
									   inner join ( select numero_ne, codigo_favorecido, it_no_mnemonico_credor as it_no_credor
												from siafi2012.ne ne 
												inner join dw.credor c ON c.it_co_credor = ne.codigo_favorecido 
												where codigo_ug_operador = '{$entungcod}' 
												and trim(plano_interno) = '{$plinumero}' 
												 ) ne ON
													numero_ne = '{$entungcod}'||(select orgcodgestao from dw.uguo where ungcod = '{$entungcod}' order by ugoid desc limit 1)||substr(foo.conta_corrente, 1,12)		
										WHERE
										naturezadet != '' and ( valor1 <> 0 OR valor2 <> 0 OR valor3 <> 0 OR valor4 <> 0 )and naturezadet ilike '3%'
										GROUP BY
										conta_corrente,codigo_favorecido, it_no_credor, naturezadet,
										conta_corrente_desc,naturezadet_desc
										ORDER BY
										conta_corrente,naturezadet,
										conta_corrente_desc,naturezadet_desc";
//										dbg(simec_htmlentities($sql1));
										$arrDados1 = $db2->carregar($sql1);
										if($arrDados1){
											$n = 0;
											foreach($arrDados1 as $dado){
												$arrLista1[$n]['cod_agrupador1'] = $dado['cod_agrupador1'];
												$arrLista1[$n]['cod_agrupador2'] = MoedaToBd($dado['cod_agrupador2']);
												$arrLista1[$n]['codigo_favorecido'] = formatar_cnpj($dado['codigo_favorecido']);
												$arrLista1[$n]['it_no_credor'] = $dado['it_no_credor'];
												$arrLista1[$n]['coluna1'] = $dado['coluna1'];
												$arrLista1[$n]['coluna2'] = $dado['coluna2'];
												$arrLista1[$n]['coluna3'] = $dado['coluna3'];
												$arrLista1[$n]['coluna4'] = $dado['coluna4'];
											$n++;
										}
									}
										
										$db2->monta_lista_array($arrLista1,$arrCabecalho,25,10,'S','center',array(),array(),'formularioempenhocusteio');//( $sql2, $arrCabecalho,25,10,'S','center','',formularioEmpCapital);
						?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloCentro" colspan="14">Empenhos Capital</td>
					</tr>
					<tr>
						<td class="SubTituloCentro" colspan="14" id="empenho_capital" >
							<?php
								$plinumero = mb_strtoupper($plinumero);
								$sql2 = "SELECT
						 						conta_corrente AS cod_agrupador1,'<span></span>' || naturezadet AS cod_agrupador2, codigo_favorecido, it_no_credor,  
										       --'' AS dsc_agrupador1,naturezadet_desc AS dsc_agrupador2,
										       sum(valor1) AS coluna1,sum(valor2) AS coluna2,sum(valor3) AS coluna3,sum(valor4) AS coluna4
										FROM
										(SELECT 
										   sld.sldcontacorrente AS conta_corrente,sld.ctecod || '.' || sld.gndcod || '.' || sld.mapcod || '.' || sld.edpcod || '.' || substr(trim(sld.sldcontacorrente), 13, 2)::character varying(2) AS naturezadet,
										   sld.sldcontacorrente AS conta_corrente_desc,ndp.ndpdsc AS naturezadet_desc,
										   CASE WHEN sld.sldcontacontabil in ('292410101', '292410402', '292410403', '292410405') THEN 
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.0435 ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor1,CASE WHEN sld.sldcontacontabil in ('292410402','292410403') THEN 
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.0435 ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor2,CASE WHEN sld.sldcontacontabil in ('292410403') THEN 
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.0435 ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor3,CASE WHEN sld.sldcontacontabil in ('292410405') THEN 
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.0435 ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor4
									       FROM
										   dw.saldo2012 sld
										   LEFT JOIN ( select distinct ctecod, gndcod, mapcod, edpcod, sbecod, ndpdsc from dw.naturezadespesa where sbecod <> '00' ) as ndp ON cast(ndp.ctecod AS text) = sld.ctecod AND cast(ndp.gndcod AS text) = sld.gndcod AND cast(ndp.mapcod AS text) = sld.mapcod AND cast(ndp.edpcod AS text) = sld.edpcod AND cast(ndp.sbecod AS text) = sld.sbecod
										   WHERE sld.ungcod in ('{$entungcod}') AND sld.plicod = '{$plinumero}' AND sld.sldcontacontabil in ( '292410101', '292410402', '292410403', '292410405', '292410402','292410403', '292410403', '292410405' )
										    UNION ALL	
										   SELECT 
										   sld.sldcontacorrente AS conta_corrente,null,
										   sld.sldcontacorrente AS conta_corrente_desc,null,
										   CASE WHEN sld.sldcontacontabil in ('292410101', '292410402', '292410403', '292410405') THEN 
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.0435 ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor1,CASE WHEN sld.sldcontacontabil in ('292410402','292410403') THEN 
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.0435 ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor2,CASE WHEN sld.sldcontacontabil in ('292410403') THEN 
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.0435 ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor3,CASE WHEN sld.sldcontacontabil in ('292410405') THEN 
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.0435 ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor4 
									       FROM 
										   dw.saldo2012 sld
										   LEFT JOIN ( select distinct ctecod, gndcod, mapcod, edpcod, sbecod, ndpdsc from dw.naturezadespesa where sbecod <> '00' ) as ndp ON cast(ndp.ctecod AS text) = sld.ctecod AND cast(ndp.gndcod AS text) = sld.gndcod AND cast(ndp.mapcod AS text) = sld.mapcod AND cast(ndp.edpcod AS text) = sld.edpcod AND cast(ndp.sbecod AS text) = sld.sbecod
										   WHERE sld.ungcod in ('{$entungcod}') AND sld.plicod = '{$plinumero}' AND sld.sldcontacontabil in ('292410101', '292410402', '292410403', '292410405', '292410402','292410403', '292410403', '292410405')
									   ) as foo
									   inner join ( select numero_ne, codigo_favorecido, it_no_credor
												from siafi2012.ne ne 
												inner join dw.credor c ON c.it_co_credor = ne.codigo_favorecido 
												where codigo_ug_operador = '{$entungcod}' 
												and trim(plano_interno) = '{$plinumero}' 
												 ) ne ON
													numero_ne = '{$entungcod}'||(select orgcodgestao from dw.uguo where ungcod = '{$entungcod}' order by ugoid desc limit 1)||substr(foo.conta_corrente, 1,12)		
										WHERE
										naturezadet != '' and ( valor1 <> 0 OR valor2 <> 0 OR valor3 <> 0 OR valor4 <> 0 )and naturezadet ilike '4%'
										GROUP BY
										conta_corrente,codigo_favorecido, it_no_credor, naturezadet,
										conta_corrente_desc,naturezadet_desc
										ORDER BY
										conta_corrente,naturezadet,
										conta_corrente_desc,naturezadet_desc";
// 										dbg(simec_htmlentities($sql2));
// 										$arrDados = $db2->carregar($sql2,null,86400);
										$arrDados = $db2->carregar($sql2);
										if($arrDados){
											$n = 0;
											foreach($arrDados as $dado){
												$arrLista[$n]['cod_agrupador1'] = $dado['cod_agrupador1'];
												$arrLista[$n]['cod_agrupador2'] = MoedaToBd($dado['cod_agrupador2']);
												$arrLista[$n]['codigo_favorecido'] = formatar_cnpj($dado['codigo_favorecido']);
												$arrLista[$n]['it_no_credor'] = $dado['it_no_credor'];
												$arrLista[$n]['coluna1'] = $dado['coluna1'];
												$arrLista[$n]['coluna2'] = $dado['coluna2'];
												$arrLista[$n]['coluna3'] = $dado['coluna3'];
												$arrLista[$n]['coluna4'] = $dado['coluna4'];
											$n++;
										}
									}
										
										$db2->monta_lista_array($arrLista,$arrCabecalho,25,10,'S','center',array(),array(),'formularioempenhocapital');//( $sql2, $arrCabecalho,25,10,'S','center','',formularioEmpCapital);
								?>
						</td>
					</tr>
					<tr>
						<td colspan="2" width="350%">
						<table class="tabela" bgcolor="#f5f5f5" cellSpacing="2" cellPadding="3" align="center">
							<?php
								$sql = "SELECT
											     --  ungcod AS cod_agrupador1,natureza AS cod_agrupador2,
											      -- ungdsc AS dsc_agrupador1,natureza_desc AS dsc_agrupador2,
											       sum(valor1) AS provisaorecebida,sum(valor2) AS destaquerecebido,sum(valor3) AS provisaoconcedida,sum(valor4) AS destaqueconcedido,sum(valor5) AS creditodisponivel
											       FROM
											       (SELECT 
												  sld.ungcod,sld.ctecod || '.' || sld.gndcod || '.' || sld.mapcod || '.' || sld.edpcod AS natureza,
												  ung.ungdsc,ndp.ndpdsc AS natureza_desc,
												  CASE WHEN sld.sldcontacontabil in ('192220100') THEN 
										CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
										ELSE 0 END AS valor1,CASE WHEN sld.sldcontacontabil in ('192210101','192210102','192210201','192210202','192210300','192210901','192210909') THEN 
										CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
										ELSE 0 END AS valor2,CASE WHEN sld.sldcontacontabil in ('292220100','292220200','292220901','292220909') THEN 
										CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
										ELSE 0 END AS valor3,CASE WHEN sld.sldcontacontabil in ('292210101','292210201','292210300','292210901','292210909') THEN 
										CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
										ELSE 0 END AS valor4,CASE WHEN sld.sldcontacontabil in ('292110000') THEN 
										CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
										ELSE 0 END AS valor5
										      FROM
												  dw.saldo2012 sld
												  LEFT JOIN dw.ug ung ON ung.ungcod = sld.ungcod LEFT JOIN ( select distinct ctecod, gndcod, mapcod, edpcod, ndpdsc from dw.naturezadespesa where sbecod = '00' ) as ndp ON cast(ndp.ctecod AS text) = sld.ctecod AND cast(ndp.gndcod AS text) = sld.gndcod AND cast(ndp.mapcod AS text) = sld.mapcod AND cast(ndp.edpcod AS text) = sld.edpcod
												  WHERE sld.acacod in ('20RX') AND sld.sldcontacontabil in ('192220100','192210101','192210102','192210201','192210202','192210300','192210901','192210909','292220100','292220200','292220901','292220909','292210101','292210201','292210300','292210901','292210909','292110000') 
													      and sld.ungcod = '{$entungcod}'
										) as foo
											       WHERE
											       ( valor1 <> 0 OR valor2 <> 0 OR valor3 <> 0 OR valor4 <> 0 OR valor5 <> 0 ) and ungcod = '{$entungcod}'
											     --  GROUP BY
											     --  ungcod,natureza,
											     --  ungdsc,natureza_desc
											     --  ORDER BY
											     --  ungcod,natureza,
											     --  ungdsc,natureza_desc";
											     
// 									$arrDados = $db2->Carregar($sql,null,86400);
									$arrDados = $db2->Carregar($sql);
									if($arrDados[0]):
										foreach($arrDados as $dado):
							?>
							
								<tr>
									<td width="32%" class="SubTituloDireita" >Crédito Descentralizado Provisão Concedida:</td>
									<td align="right">
										<?=number_format($dado['provisaoconcedida'],2,',','.') ?>
									</td>
								</tr>
								<tr>
									<td width="25%" class="SubTituloDireita">Crédito Descentralizado Provisão Recebida:</td>
									<td align="right">
										<?=number_format($dado['provisaorecebida'],2,',','.') ?>
									</td>
								</tr>
								<tr>
									<td width="25%" class="SubTituloDireita">Crédito Descentralizado Destaque Concedido:</td>
									<td align="right">
										<?=number_format($dado['destaqueconcedido'],2,',','.') ?>
									</td>
								</tr>
								<tr>
									<td width="25%" class="SubTituloDireita">Crédito Descentralizado Destaque Recebido:</td>
									<td align="right">
										<?=number_format($dado['destaquerecebido'],2,',','.') ?>
									</td>
								</tr>
								<tr>
									<td width="25%" class="SubTituloDireita">Crédito Disponível:</td>
									<td align="right">
										<?=number_format($dado['creditodisponivel'],2,',','.') ?>
									</td>
								</tr>
								<?php
									endforeach;
								endif;
								 ?>
						 </table>
								 </td>
							</tr>
					 </table>
				
		
			<form method="POST" id="formulario" name="formulario">
				<input type="hidden" name="qrptitulo" id="qrptitulo" value="Questionário Custeio/Capital - <?php echo $entungcod ?> - <?php echo $arrPortariasentidade['ifes'] ?>">
				<input type="hidden" name="docid" id="docid" value="<?=$docid ?>">
				<input type="hidden" name="usucpf" id="usucpf" value="<?=$_SESSION['usucpf'] ?>">
				<input type="hidden" name="qrpdata" id="qrpdata" value="<?=$dataAtualHora ?>">
				<input type="hidden" name="vpiid" id="vpiid" value="<?=$vpiid ?>">
				<input type="hidden" name="entid" id="entid" value="<?=$entid ?>">
				<input type="hidden" name="prtid" id="prtid" value="<?=$prtid ?>">
				<input type="hidden" name="pltid" id="pltid" value="<?=$pltid ?>">
				<input type="hidden" name="prcug" id="prcug" value="<?=$entungcod ?>">
				<input type="hidden" name="prcid" id="prcid" value="<?=$prcid ?>">
				<input type="hidden" name="pcdid" id="pcdid" value="<?=$pcdid?>">
				<input type="hidden" name="pcrid" id="pcrid" value="<?=$pcrid?>">
				<input type="hidden" name="dataatualizacao" id="dataatualizacao" value="now()">
				<input type="hidden" name="requisicao" value="salvarPrestacaoContas">
				<table>
						<tr>
							<td class="SubTituloCentro" colspan="8">
								<?php 
									if($esdid == ESTADO_BLOQUEADO){
										echo $mensagem;
									}
								?>
							</td>
						<tr>
					<tr>
						<td class="SubTituloCentro" colspan="8">Comentários</td>
					</tr>
					<?php
					if($arrVpqid[0]):
						$sql = "SELECT 
									qp.perid,
									qp.pertitulo,
									qp.queid
								FROM 
									rehuf.vinculointernoquestionario vpiq
								inner join 
									questionario.pergunta qp ON vpiq.queid=qp.queid
								WHERE vpqid in(".implode(",",$arrVpqid).")
								ORDER BY qp.perid";
						$arrQuestionario = $db->carregar($sql);
						if($arrQuestionario):
						 	$n = 0; 
							$numero = 1;
							foreach($arrQuestionario as $dado): ?>
								<tr>
									<td width="25%" align="center"><?= $numero.' - '.$dado['pertitulo'] ?></td>
									<td width="25%" align="center">
										<input type="hidden" name="hdn_queid[<?php echo $dado['queid'] ?>]" value="<?php echo $dado['queid'] ?>" />
										<input type="hidden" name="hdn_perid[<?php echo $dado['queid'] ?>][<?php echo $n ?>]" value="<?php echo $dado['perid'] ?>" />
										<input type="hidden" name="hdn_qrpid[<?php echo $dado['queid'] ?>][<?php echo $n ?>]" value="<?php echo $arrRespostasQuestionario[$dado['queid']][$dado['perid']]['qrpid'] ?>" />
										<input type="hidden" name="hdn_resid[<?php echo $dado['queid'] ?>][<?php echo $n ?>]" value="<?php echo $arrRespostasQuestionario[$dado['queid']][$dado['perid']]['resid'] ?>" />
										<?=campo_textarea( "campo_".$dado['queid']."_".$n, $habil, $ob, '', '100', '4', '5000','','','','','',$arrRespostasQuestionario[$dado['queid']][$dado['perid']]['resdsc']);?>
									</td>
								</tr>
								<?php $n++; 
									  $numero++;?>
							<?php endforeach; ?>
						<?php endif; 
					endif;
					?>
					<tr>
						<td colspan="2">
							<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>
								<?php
									$sql = "select distinct
														segu.usucpf as cpf,
														segu.usunome as nome,
														prc.dataatualizacao as data
										from 
													seguranca.usuario segu
										inner join 
													rehuf.prestacaocontas prc on prc.usucpf = segu.usucpf
										inner join 
													rehuf.vinculoplanointerno vpi on vpi.vpiid = prc.vpiid
										where 
													vpi.vpiid = {$vpiid}
										and
													prc.prcug = '{$entungcod}'";
													
										$assinatura = $db->pegaLinha($sql);
										if($assinatura):
									?>
									<tr>
										<td class="SubTituloCentro" colspan="14">Assinatura</td>
									</tr>
										<?php if( in_array($esdid, array(ESTADO_EM_ELABORACAO,ESTADO_BLOQUEADO))): ?>
									<tr>
										<td class="SubTituloCentro" colspan="14">Última Atualização: <?=$assinatura['nome']; ?> - <?= formatar_cpf($assinatura['cpf']); ?> - <?=strftime("%d/%m/%Y %H:%M:%S", strtotime($assinatura['data'])); ?></td>
									</tr>
										<?php else: ?>
									<tr>
										<td class="SubTituloCentro" colspan="14">Enviado por: <?=$assinatura['nome']; ?> - <?= formatar_cpf($assinatura['cpf']); ?> - <?=strftime("%d/%m/%Y %H:%M:%S", strtotime($assinatura['data'])); ?></td>
									</tr>
									<?php endif; ?>
							<?php endif; ?>
							</table>
						</td>
					</tr>
					 
					<tr>
						<td width="25%" class="SubTituloDireita"></td>
							<td>
								<input type="button" name="btn_voltar" value="Voltar" onclick="voltar()" />
								<?php
									$data=date("Ymd");
									if($habilitaBotãoSalvar == 'S'):
//									if(1==1):
								?>
									<input type="button" name="btn_salvar" value="Salvar"  onclick="salvarPrestacaoContas()" />
								<?php else: ?>
								
									<input type="button" name="btn_salvar" value="Salvar" disabled="disabled" />
								<?php endif; ?>
								<?php if( in_array($esdid, array(ESTADO_ENCERRADO))):
//										if(1==1): 
								?>
									<input type="button" name="btn_salvar" value="Imprimir PDF" onclick="geraPDF()" />
								<?php else: ?>
									<input type="button" name="btn_salvar" value="Imprimir PDF" disabled="disabled" />
								<?php endif; ?>
							</td>
						</tr>
				</table>
			</form>
		</td>
		<td valign="top">
			<?php wf_desenhaBarraNavegacao( $docid, array("prcid"=>$prcid,"vpiid"=>$vpiid));?>
			<table border="0" cellpadding="3" cellspacing="0"
				style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px; margin-bottom: 20px;">
				<tr style="background-color: #c9c9c9; text-align: center;">
					<td style="font-size: 7pt; text-align: center;"><span
						title="Cadastramento de parecer"> <strong>Outras Ações</strong> </span></td>
				</tr>
				<tr style="text-align: center;">
					<td style="font-size: 7pt; text-align: center;">
					<a style="cursor: pointer" href="<?php $_SERVER['name']?>/rehuf/documento/tutorial.doc" >Download (Passo a passo DAF)</a></td>
				</tr>
			</table>
		</td>
	</tr>
</table>
