<?php

// salva os POST na tabela
if ( $_REQUEST['salvar'] == 1 ){
	$existe_rel = 0;
	$sql = sprintf(
		"select prtid from public.parametros_tela where prtdsc = '%s'",
		$_REQUEST['titulo']
	);
	$existe_rel = $db->pegaUm( $sql );
	if ($existe_rel > 0) 
	{
		/*
		?>
		<script language="text/javascript">
			if ( !confirm( 'Deseja alterar a consulta já existente?' ) )
			{
				history.back();
			}
		</script>
		<?
		*/
		$sql = sprintf(
			"UPDATE public.parametros_tela SET prtdsc = '%s', prtobj = '%s', prtpublico = 'FALSE', usucpf = '%s', mnuid = %d WHERE prtid = %d",
			$_REQUEST['titulo'],
			addslashes( addslashes( serialize( $_REQUEST ) ) ),
			$_SESSION['usucpf'],
			$_SESSION['mnuid'],
			$existe_rel
		);
		$db->executar( $sql );
		$db->commit();
	}
	else 
	{
		$sql = sprintf(
			"INSERT INTO public.parametros_tela ( prtdsc, prtobj, prtpublico, usucpf, mnuid ) VALUES ( '%s', '%s', %s, '%s', %d )",
			$_REQUEST['titulo'],
			addslashes( addslashes( serialize( $_REQUEST ) ) ),
			'FALSE',
			$_SESSION['usucpf'],
			$_SESSION['mnuid']
		);
		$db->executar( $sql );
		$db->commit();
	}
	?>
	<script type="text/javascript">
		alert('Operação realizada com sucesso!');
		location.href = '?modulo=relatorio/relatorio_geral&acao=A';
	</script>
	<?
	die;
}

/*
//recupera perfil do usuário
$perfil = arrayPerfil();


//coloca periodo e valor do ponto no titulo
if(!$_REQUEST['titulo']) $_REQUEST['titulo'] = "Relatório de Demandas";

if ( in_array(DEMANDA_PERFIL_SUPERUSUARIO, $perfil) || in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil) || in_array(DEMANDA_PERFIL_ANALISTA_SISTEMA, $perfil) || in_array(DEMANDA_PERFIL_GESTOR_REDES, $perfil) || in_array(DEMANDA_PERFIL_DBA, $perfil) || in_array(DEMANDA_PERFIL_ADMINISTRADOR, $perfil) || in_array(DEMANDA_PERFIL_ANALISTA_WEB, $perfil) || in_array(DEMANDA_PERFIL_GESTOR_MEC, $perfil) || in_array(DEMANDA_PERFIL_ANALISTA_TECNICO, $perfil) ){
	$valorpto = $db->pegaUm("select vlpvalorponto from demandas.valorpontuacao");
	if($valorpto){
		$valorpto = "<br><font style='font-size:10px;'>Valor do Ponto: R$ ".number_format($valorpto,2,",",".")."</font>";
	}
	else{
		$valorpto = "<br><font style='font-size:10px;'>Valor do Ponto: R$ 0,00</font>";
	}
}

$txtdtinifim = "";
if( $_REQUEST['dtinicio'] && $_REQUEST['dtfim'] ) $txtdtinifim = "<br><font style='font-size:10px;'>Período de Abertura: ".$_REQUEST['dtinicio']." à ".$_REQUEST['dtfim']."</font>";

$txtdtinifimsit = "";
if( $_REQUEST['dtinisit'] && $_REQUEST['dtfimsit'] ) $txtdtinifimsit = "<br><font style='font-size:10px;'>Período da Situação: ".$_REQUEST['dtinisit']." à ".$_REQUEST['dtfimsit']."</font>";

$_REQUEST['titulo'] = $_REQUEST['titulo'] . $valorpto . $txtdtinifim . $txtdtinifimsit;
*/

 

// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';


$sql   = monta_sql();
//dbg($sql,1);
$dados = $db->carregar($sql);

ini_set("memory_limit", "5120M");

$colunax = (array) $_REQUEST['coluna'];

// se existir a coluna artefatos no relatorio
if (in_array("artefatos", $colunax)){

    foreach($dados as &$dado){

        // seleciona todos os artefatos da ss
        $sql = "SELECT
                        ans.scsid, ans.ansid,
                        tpe.tpeid, tpe.tpedsc,
                        dsp.dspid, dsp.dspdsc,
                        fas.fasid, fas.fasdsc,
                        prd.prdid, prd.prddsc
                FROM
                        fabrica.analisesolicitacao ans
                INNER JOIN
                        fabrica.servicofaseproduto sfp
                        on sfp.ansid=ans.ansid
                INNER JOIN
                        fabrica.tipoexecucao tpe
                        on tpe.tpeid=sfp.tpeid
                INNER JOIN
                        fabrica.fasedisciplinaproduto fdp
                        on fdp.fdpid=sfp.fdpid
                INNER JOIN
                        fabrica.produto prd
                        on prd.prdid=fdp.prdid
                INNER JOIN
                        fabrica.fasedisciplina fsd
                        on fsd.fsdid=fdp.fsdid
                INNER JOIN
                        fabrica.disciplina dsp
                        on dsp.dspid=fsd.dspid AND dsp.dspid=prd.dspid
                INNER JOIN
                        fabrica.fase fas
                        on fas.fasid=fsd.fasid
                WHERE
                        ans.scsid={$dado['nss']}
                ORDER BY
                        ans.scsid,
                        ans.ansid,
                        tpe.tpeid,
                        dsp.dspid,
                        fas.fasid,
                        prd.prdid";


        $artefatos = $db->carregar($sql);
        $dadosArtefato = array();
        $htmlArtefato = "<table cellSpacing='1' cellPadding='3' width='100%'>";

                        
        if($artefatos){

            // criando estrutura de artefatos por ss
            foreach($artefatos as $artefato){

                $dadosArtefato[$artefato['tpedsc']][$artefato['dspdsc']][$artefato['fasdsc']][$artefato['prddsc']] = true;

            }

            // lendo a estrutura de artefato e montando o html para ser exibido
            foreach($dadosArtefato as $tpedsc=>$disciplinas){

                $htmlArtefato .= "<tr><td width='100%'>$tpedsc<hr></td></tr>";

                $htmlArtefato .= "<tr><td width='100%'>";

                foreach($disciplinas as $dspdsc=>$fases){

                    $htmlArtefato .= "<span style='padding-left:40px'><b>$dspdsc</b></span><br/>";

                    foreach($fases as $fasdsc=>$produtos){

                        $htmlArtefato .= "<span style='padding-left:60px'><b> - $fasdsc</b></span><br/>";

                        foreach($produtos as $prddsc=>$vazio){

                            $htmlArtefato .= "<div style='padding-left:80px'> - $prddsc</div>";

                        }


                    }

                }

                $htmlArtefato .= "</td></tr>";

            }

            $htmlArtefato .= "</table>";


            // adicionando no objeto de dados da consulta a coluna artefatos
            $dado['artefatos'] = $htmlArtefato;

        }else{

            $dado['artefatos'] = "N/D";

        }
    }
}

$agrup = monta_agp();
$col   = monta_coluna();
$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados); 
$r->setColuna($col);
$r->setTotNivel(true);
$r->setBrasao(true);
if($_REQUEST['tiporel'] == '2'){
	$r->setEspandir(false);
}else{
	$r->setEspandir(true);
}


//xls
if($_REQUEST['pesquisa'] == '2'){
	$nomeDoArquivoXls = "SIMEC_FABRICA_REL_GERAL_".date("YmdHis");
	echo $r->getRelatorioXls();
}

?>


<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>

	<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	

		<!--  Monta o Relatório -->
		<? echo $r->getRelatorio(); ?>

	</body>
</html>

<script>
	function chamaproc(prcid)
	{
		//window.opener.location="conjur.php?modulo=principal/editarprocesso&acao=A&prcid="+prcid;
		window.opener.focus();
		
	}
</script>
<?

function monta_sql(){
	global $db;
	extract($_REQUEST);

	$where = array();
		
	// tiposervico
	if( $tiposervico[0] && $tiposervico_campo_flag != '' ){
		array_push($where, " ts.tpsid " . (!$tiposervico_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode( ',', $tiposervico ) . ") ");
	}

	// Tipo da Ordem de Serviço
	if( $tipoos[0] && $tipoos_campo_flag != '' ){
		array_push($where, " tos.tosid " . (!$tipoos_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $tipoos ) . "') ");
	}
	
	// requisitante
	if( $requisitante[0]  && $requisitante_campo_flag != '' ){
		array_push($where, " ss.usucpfrequisitante " . (!$requisitante_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $requisitante ) . "') ");
	}

	// sistema
	if( $sistema[0] && $sistema_campo_flag != '' ){
		array_push($where, " ss.sidid " . (!$sistema_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $sistema ) . "') ");
	}

	// situacao da SS
	if( $sitss[0] && $sitss_campo_flag != '' ){
		array_push($where, " ess.esdid " . (!$sitss_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode( ',', $sitss ) . ") ");
	}
	
	/*
	// situacao da demanda
	if( $situacao[0] && $situacao_campo_flag != '' ){
		array_push($where, " ed.esdid " . (!$situacao_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode( ',', $situacao ) . ") ");
	}	
	*/
	
	// período da situação SS
	if( $dtiniss && $dtfimss ){
		$dtiniss = formata_data_sql($dtiniss);
		$dtfimss = formata_data_sql($dtfimss);
		array_push($where, " a.htddata between '{$dtiniss} 00:00:00' and '{$dtfimss} 23:59:59' ");
	}
	
	
	// período de abertura da SS
	if( $dtaberturaini && $dtaberturafim ){
		$dtaberturaini = formata_data_sql($dtaberturaini);
		$dtaberturafim = formata_data_sql($dtaberturafim);
		array_push($where, " ss.dataabertura BETWEEN '{$dtaberturaini} 00:00:00' AND '{$dtaberturafim} 23:59:59' ");
	}

	// período de expectativa da SS
	if( $dtexpectativaini && $dtexpectativafim ){
		$dtexpectativaini = formata_data_sql($dtexpectativaini);
		$dtexpectativafim = formata_data_sql($dtexpectativafim);
		array_push($where, " ss.scsprevatendimento BETWEEN '{$dtexpectativaini} 00:00:00' AND '{$dtexpectativafim} 23:59:59' ");
	}

	// período previsto de termino da SS
	if( $dtterminoini && $dtterminofim ){
		$dtterminoini = formata_data_sql($dtterminoini);
		$dtterminofim = formata_data_sql($dtterminofim);
		array_push($where, " os.odsdtprevtermino BETWEEN '{$dtterminoini} 00:00:00' AND '{$dtterminofim} 23:59:59' ");
	}
	
	//Intervalo Qtd. de PF Estimada
	if($qtdpfeini && $qtdpfefim){
		array_push($where, " ( os.odsqtdpfestimada >= {$qtdpfeini} AND os.odsqtdpfestimada <= {$qtdpfefim} ) ");
	}	
	
	//Intervalo Qtd. de PF Detalhada
	if($qtdpfdini && $qtdpfdfim){
		array_push($where, " ( os.odsqtdpfdetalhada >= {$qtdpfdini} AND os.odsqtdpfdetalhada <= {$qtdpfdfim} ) ");
	}	
	
	
	// pesquisa em campos textos
	if( $pesquisacampo && $textopesquisa ){

		$textopesquisa = str_replace("'", "", $textopesquisa);
		
		if(in_array("necessidade", $pesquisacampo)) array_push($where, " upper(ss.scsnecessidade) ilike '%".strtoupper($textopesquisa)."%' ");
		if(in_array("justificativa", $pesquisacampo)) array_push($where, " upper(ss.scsjustificativa) ilike '%".strtoupper($textopesquisa)."%' ");
		if(in_array("nss", $pesquisacampo)) array_push($where, " ss.scsid = '".strtoupper($textopesquisa)."' ");
		if(in_array("nos", $pesquisacampo)) array_push($where, " os.odsid = '".strtoupper($textopesquisa)."' ");
		//array_push($where, " ( upper(d.dmdtitulo) like '%".strtoupper($textopesquisa)."%' OR  upper(d.dmddsc) like '%".strtoupper($textopesquisa)."%' ) ");
	}
	
	
	//ORDENAR CAMPOS
	if($_REQUEST['ordenarcampo']){
		$ORDENA = "";
		$ORDENA = implode(', ', $_REQUEST['ordenarcampo']);
		//$ORDENA = str_replace("previsaotermino"," ".$_REQUEST['tipoordenar'],$ORDENA);
		
	}else{
		$ORDENA = implode(', ', $_REQUEST['agrupador']);
		//$ORDENA = str_replace("artefatos"," ".$_REQUEST['tipoordenar'],$ORDENA);
	}	
	
	//SQL tunada atual
	$sql = "select distinct 
				ss.scsid as nss, 
				os.odsid as nos, 
				an.ansid as ansid,
				to_char(ss.dataabertura::timestamp,'DD/MM/YYYY HH24:MI:SS') as dataaberturass,
				sis.siddescricao as sistema,
				u.usunome as requisitante,
				UPPER(ug.ungdsc) as unidade,
				ss.scsnecessidade as necessidade,
				ss.scsjustificativa as justificativa,
				to_char(ss.scsprevatendimento::timestamp,'DD/MM/YYYY HH24:MI:SS') as expectativaatendimentoss,
				to_char(os.odsdtprevinicio::timestamp,'DD/MM/YYYY HH24:MI:SS') as previsaoinicioos,
				to_char(os.odsdtprevtermino::timestamp,'DD/MM/YYYY HH24:MI:SS') as previsaoterminoos,
				to_char(a.htddata::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS dtss,
				eos.esddsc as situacaoos,
				eospf.esddsc as situacaoosc,
				ess.esddsc as situacaoss,
				ts.tpsdsc as tiposervico,
				tos.tosdsc as tipoordemservico,
				os.odsqtdpfestimada as qtdpfestimada,
				os.odsqtdpfdetalhada as qtdpfdetalhada,
				'' as artefatos 
		from fabrica.solicitacaoservico ss
		left join fabrica.analisesolicitacao an on an.scsid = ss.scsid
		left join fabrica.tiposervico ts on ts.tpsid = an.tpsid
		left join fabrica.ordemservico os on os.scsid = ss.scsid
		left join fabrica.tipoordemservico tos on tos.tosid = os.tosid
		left join workflow.documento dos on dos.docid = os.docid
		left join workflow.estadodocumento eos on eos.esdid = dos.esdid
		left join workflow.documento dospf on dospf.docid = os.docidpf
		left join workflow.estadodocumento eospf on eospf.esdid = dospf.esdid
		left join workflow.documento dss on dss.docid = ss.docid
		left join workflow.estadodocumento ess on ess.esdid = dss.esdid
		LEFT JOIN workflow.historicodocumento a ON a.hstid = dss.hstid
		left join demandas.sistemadetalhe sis on sis.sidid = ss.sidid
		left join seguranca.usuario u on u.usucpf = ss.usucpfrequisitante
		left join public.unidadegestora ug on ug.ungcod = u.ungcod
		
		WHERE ss.scsstatus = 'A' " . ( $where[0] ? ' AND' . implode(' AND ', $where) : '' ) . "
		ORDER BY  {$ORDENA}, ss.scsid
                
		 ";		
	//dbg($sql,1);
	return $sql;
}

function monta_agp(){
	$agrupador = $_REQUEST['agrupador'];
	//dbg($agrupador,1);
	
	//if (in_array("nudemanda", $agrupador)){
		$agp = array(
					"agrupador" => array(),
					"agrupadoColuna" => array(
												"sistema",
												"situacaoss",
												"situacaoos",
												"situacaoosc",
												"requisitante",
												"unidade",
												"tiposervico",
												"tipoordemservico",
												"nss",
												"nos",
												"dataaberturass",
												"expectativaatendimentoss",
												"previsaoinicioos",
												"previsaoterminoos",
												"justificativa",
												"necessidade",
												"qtdpfestimada",
												"qtdpfdetalhada",
												"artefatos",
												"dtss",
												"sitss"
												
											  )	  
					);
	foreach ($agrupador as $val): 
	
	
		switch ($val) {		
		    case 'sistema':
				array_push($agp['agrupador'], array(
													"campo" => "sistema",
											 		"label" => "Sistema")										
									   				);					
		    	continue;			
		        break;	
		        
		    case 'situacaoss':
				array_push($agp['agrupador'], array(
												"campo" => "situacaoss",
												"label" => "Situação da SS")										
										   		);	
				continue;
				break;

		    case 'situacaoos':
				array_push($agp['agrupador'], array(
												"campo" => "situacaoos",
												"label" => "Situação da OS")										
										   		);	
				continue;
				break;
				
		    case 'situacaoosc':
				array_push($agp['agrupador'], array(
												"campo" => "situacaoosc",
												"label" => "Situação da OS Contagem")										
										   		);	
				continue;
				break;
				
		    case 'requisitante':
				array_push($agp['agrupador'], array(
												"campo" => "requisitante",
												"label" => "Requisitante")										
										   		);	
				continue;
				break;
				
		    case 'unidade':
				array_push($agp['agrupador'], array(
												"campo" => "unidade",
												"label" => "Unidade do Requisitante")										
										   		);	
				continue;
				break;	 

		    case 'tiposervico':
				array_push($agp['agrupador'], array(
												"campo" => "tiposervico",
												"label" => "Tipo de Serviço")										
										   		);	
				continue;
				break;	 
				
		    case 'tipoordemservico':
				array_push($agp['agrupador'], array(
												"campo" => "tipoordemservico",
												"label" => "Tipo da Ordem de Serviço")										
										   		);	
				continue;
				break;	 
				
				
		    case 'nss':
				array_push($agp['agrupador'], array(
												"campo" => "nss",
												"label" => "Nº SS")										
										   		);	
				continue;
				break;	 

		    case 'nos':
				array_push($agp['agrupador'], array(
												"campo" => "nos",
												"label" => "Nº OS")										
										   		);	
				continue;
				break;	 
			
		}
	endforeach;

	/*
	array_push($agp['agrupador'], array(
									"campo" => "nss",
							  		"label" => "Nº SS")										
					   				);				
	*/
	
	//dbg($agrupador,1);
	return $agp;
}


function monta_coluna(){
	$colunaArr = (array) $_REQUEST['coluna'];
	
	
	$coluna = array();
	
	foreach($colunaArr as $campo):
		switch ($campo) {	
			
			case 'dtss':
				array_push($coluna,array(
													  "campo" => "dtss",
											   		  "label" => "Data da Situação SS"
													)										
									   			);					
		    	continue;			
		        break;
		        
		    case 'dataaberturass':
				array_push($coluna,array(
													  "campo" => "dataaberturass",
											   		  "label" => "Data de Abertura da SS"
													)										
									   			);					
		    	continue;			
		        break;	
		        
		    case 'expectativaatendimentoss':
				array_push($coluna, array(
													  "campo" => "expectativaatendimentoss",
											   		  "label" => "Expectativa de Atendimento da SS"	
													)										
										   		);	
				continue;
				break;

		    case 'previsaoinicioos':
				array_push($coluna, array(
														"campo" => "previsaoinicioos",
												   		"label" => "Previsão de início da OS"
													)										
										   		);	
				continue;
				break;
				
		    case 'previsaoterminoos':
				array_push($coluna, array(
														"campo" => "previsaoterminoos",
												   		"label" => "Previsão de término da OS"
													)										
										   		);	
				continue;
				break;

			case 'tipoordemservico':
				array_push($coluna, array(
														"campo" => "tipoordemservico",
												   		"label" => "Tipo da Ordem de Serviço"
													)										
										   		);	
				continue;
				break;
				
			case 'qtdpfestimada':
				array_push($coluna, array(
														"campo" => "qtdpfestimada",
												   		"label" => "Qtd. PF Estimada"
													)										
										   		);	
				continue;
				break;
				
			case 'qtdpfdetalhada':
				array_push($coluna, array(
														"campo" => "qtdpfdetalhada",
												   		"label" => "Qtd. PF Detalhada"
													)										
										   		);	
				continue;
				break;

			case 'artefatos':
				array_push($coluna, array(
														"campo" => "artefatos",
												   		"label" => "Artefatos"
													)										
										   		);	
				continue;
				break;
				
		    case 'nss':
				array_push($coluna, array(
														"campo" => "nss",
												   		"label" => "Nº SS",
														"type"  => "string"	
													)										
										   		);	
				continue;
				break;

		    case 'nos':
				array_push($coluna, array(
														"campo" => "nos",
												   		"label" => "Nº OS",
														"type"  => "string"	
													)										
										   		);	
				continue;
				break;
				
		 }
	endforeach;		

	return $coluna;			  	
}
?>