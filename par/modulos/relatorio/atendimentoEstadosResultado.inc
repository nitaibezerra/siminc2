<?php
ini_set("memory_limit","250M");
set_time_limit(0);

if( $_REQUEST['rgfnumconvenio'] ){
	
	$sql = "SELECT rgfvigenciaconvenio FROM cte.relatoriogeralfinanceiro WHERE rgfnumconvenio = ".$_REQUEST['rgfnumconvenio'];
	$data = formata_data( $db->pegaUm( $sql ));
	
	$titulo = "<table cellspacing='0' cellpadding='2' border='0' align='center' width='100%' class='listagem'>
				<tr> <td align='center'> <b>VIGÊNCIA</b> </td> </tr>
				<tr> <td> {$data} </td> </tr>
			</table>";
	
	echo $titulo;
	exit();
}

if( $_REQUEST['painel'] ){
	
	if( $_REQUEST['painel'] == 'dstqtdpainel' ){
		$var = "Qtd. Painel";
		$select = "SUM((CASE WHEN (".$_REQUEST['painel']." IS NULL ) OR (".$_REQUEST['painel']." = '') THEN '0' ELSE ".$_REQUEST['painel']." END)::integer) as qtd";
	} elseif( $_REQUEST['painel'] == 'dstqtdmonitorada' ){
		$var = "Qtd. Monit.";
		$select = "SUM((CASE WHEN (".$_REQUEST['painel']." IS NULL ) OR (".$_REQUEST['painel']." = '') THEN '0' ELSE ".$_REQUEST['painel']." END)::integer) as qtd";
	} elseif( $_REQUEST['painel'] == 'dstindicadorpainel' ){
		$var = "Indicador Painel";
		$select = "SUM((CASE WHEN (".$_REQUEST['painel']." IS NULL ) OR (".$_REQUEST['painel']." = '') THEN '0' ELSE ".$_REQUEST['painel']." END)::integer) as qtd";
	} elseif( $_REQUEST['painel'] == 'dstqtdoutrafonte' ){
		$var = "Qtd. Outra Fonte";
		$select = "SUM((CASE WHEN (".$_REQUEST['painel']." IS NULL ) OR (".$_REQUEST['painel']." = '') THEN '0' ELSE ".$_REQUEST['painel']." END)::integer) as qtd";
	} elseif( $_REQUEST['painel'] == 'quantidadeGlobalPlanejado' ){
		$var = "Planejado";
		$select = "SUM((CASE WHEN (".$_REQUEST['painel']." IS NULL ) THEN '0' ELSE (quantidadePorEscolaPlanejado + quantidadeGlobalPlanejado) END)::integer) as qtd";
	} elseif( $_REQUEST['painel'] == 'quantidadeGlobalAprovado' ){
		$var = "Aprovado";
		$select = "SUM((CASE WHEN (".$_REQUEST['painel']." IS NULL ) THEN '0' ELSE (quantidadePorEscolaAprovado + quantidadeGlobalAprovado) END)::integer) as qtd";
	} elseif( $_REQUEST['painel'] == 'quantidadeGlobalEmanalise' ){
		$var = "Em Análise";
		$select = "SUM((CASE WHEN (".$_REQUEST['painel']." IS NULL ) THEN '0' ELSE (quantidadePorEscolaEmanalise + quantidadeGlobalEmanalise) END)::integer) as qtd";
	}
	
	$sql = "SELECT estuf AS estado, undid, unddsc, ".$select."
			FROM (  -- POR ESCOLA --
				SELECT           
					iu.estuf,
					s.sbaid,
					s.sbadsc,
					u.unddsc,
					u.undid,
					dst.dstqtdpainel,
					dst.dstindicadorpainel,
					dst.dstqtdoutrafonte,
					dst.dstfonte,
					0 as quantidadeGlobalPlanejado,
					0 as quantidadeGlobalAprovado,
					0 as quantidadeGlobalEmanalise,
					sum(coalesce(qf.qfaqtd,0)) as quantidadePorEscolaPlanejado,
					CASE WHEN spt.ssuid = 3  THEN sum(coalesce(qf.qfaqtd,0)) ELSE 0 END as quantidadePorEscolaAprovado,
					CASE WHEN spt.ssuid = 1  THEN sum(coalesce(qf.qfaqtd,0)) ELSE 0 END as quantidadePorEscolaEmanalise,
					d.dimcod || '.' || ad.ardcod || '.' || i.indcod || '.' || COALESCE(s.sbaordem, 0) as codigo,
					dst.dstqtdmonitorada
				FROM cte.dimensao d
				INNER JOIN cte.areadimensao ad ON ad.dimid = d.dimid
				INNER JOIN cte.indicador i ON i.ardid = ad.ardid
				INNER JOIN cte.criterio c ON c.indid = i.indid
				INNER JOIN cte.pontuacao p ON p.crtid = c.crtid AND p.indid = i.indid
				INNER JOIN cte.instrumentounidade iu ON iu.inuid = p.inuid
				INNER JOIN cte.acaoindicador a ON a.ptoid = p.ptoid
				INNER JOIN cte.subacaoindicador s ON s.aciid = a.aciid
				INNER JOIN cte.unidademedida u on u.undid = s.undid
				INNER JOIN cte.qtdfisicoano qf ON qf.sbaid = s.sbaid 
				LEFT  JOIN cte.dadossubacaotecnica dst ON dst.sbaid = s.sbaid
				LEFT  JOIN cte.subacaoparecertecnico spt ON spt.sbaid = s.sbaid AND qf.qfaano = spt.sptano
				WHERE
					s.sbaporescola = true -- por escola.
					AND s.frmid in (4,6) -- and s.sbaid = 2361
					AND iu.estuf = '".$_SESSION['par']['postrelat']['estado']."'
				GROUP BY 
					iu.estuf,
					s.sbaid,
					s.sbadsc,
					u.unddsc,
					u.undid,
					spt.ssuid,
					dst.dstqtdpainel,
					dst.dstindicadorpainel,
					dst.dstqtdoutrafonte,
					dst.dstfonte,
					d.dimcod, 
					ad.ardcod,
					i.indcod,
					s.sbaordem,
					dst.dstqtdmonitorada
				UNION ALL
		            -- GLOBAL --
				SELECT           
					iu.estuf,
					s.sbaid,
                    s.sbadsc,
                    u.unddsc,
                    u.undid,
                    dst.dstqtdpainel,
                    dst.dstindicadorpainel,
                    dst.dstqtdoutrafonte,
                    dst.dstfonte,
                    sum(coalesce(spt.sptunt,0)) as quantidadeGlobalPlanejado,
                    CASE WHEN spt.ssuid = 3  THEN sum(coalesce(spt.sptunt,0)) ELSE 0 END as quantidadeGlobalAprovado,
                    CASE WHEN spt.ssuid = 1  THEN sum(coalesce(spt.sptunt,0)) ELSE 0 END as quantidadeGlobalEmanalise,
                    0 as quantidadePorEscolaPlanejado,
                    0 as quantidadePorEscolaAprovado,
                    0 as quantidadePorEscolaEmanalise,
                    d.dimcod || '.' || ad.ardcod || '.' || i.indcod || '.' || COALESCE(s.sbaordem, 0) as codigo,
                    dst.dstqtdmonitorada
				FROM cte.dimensao d
		        INNER JOIN cte.areadimensao 			ad 	ON ad.dimid = d.dimid
		        INNER JOIN cte.indicador 				i 	ON i.ardid = ad.ardid
		        INNER JOIN cte.criterio 				c 	ON c.indid = i.indid
		        INNER JOIN cte.pontuacao 				p	ON p.crtid = c.crtid AND p.indid = i.indid AND p.ptostatus = 'A'
		        INNER JOIN cte.instrumentounidade 		iu 	ON iu.inuid = p.inuid
		        INNER JOIN cte.acaoindicador 			a 	ON a.ptoid = p.ptoid
		        INNER JOIN cte.subacaoindicador 		s 	ON s.aciid = a.aciid
		        INNER JOIN cte.unidademedida 			u 	on u.undid = s.undid
		        LEFT  JOIN cte.dadossubacaotecnica 		dst ON dst.sbaid = s.sbaid
		        LEFT  JOIN cte.subacaoparecertecnico 	spt ON spt.sbaid = s.sbaid 
				WHERE
					s.sbaporescola = false -- global.
					AND s.frmid in (4,6)
					AND iu.estuf = '".$_SESSION['par']['postrelat']['estado']."'
				GROUP BY 
					iu.estuf, 
					s.sbaid,
					s.sbadsc,
					u.unddsc, u.undid, spt.ssuid,
					dst.dstqtdpainel,
					dst.dstindicadorpainel,
					dst.dstqtdoutrafonte,
					dst.dstfonte,
					d.dimcod, 
					ad.ardcod,
					i.indcod,
					s.sbaordem,
					dst.dstqtdmonitorada
		) AS foo
		GROUP BY	estuf, undid, unddsc
		ORDER BY 	estuf";

	$dados = $db->carregar( $sql );
	
	$titulo = "<table cellspacing='0' cellpadding='2' border='0' align='center' width='100%' class='listagem'>
				<tr> 
					<td align='center'> <b>Unidade de Medida</b> </td>
					<td align='center'> <b>".$var."</b> </td>
				</tr> ";
	
	if( is_array( $dados ) ){
		foreach( $dados as $dado ){
			if( $dado['qtd'] != 0 ){
				 $titulo.= "<tr>
								<td> {$dado['unddsc']} </td>
								<td> {$dado['qtd']} </td>
							</tr>";
			}
		}
	} 
	
	$titulo.= "</table>";
	
	echo $titulo;
	exit();
}

if ( $_REQUEST['filtrosession'] ){
	$filtroSession = $_REQUEST['filtrosession'];
}

if ($_POST['agrupador']){
	header( 'Content-Type: text/html; charset=iso-8859-1' ); 
}
?>
<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<!-- Includes do SuperTitleAjax -->
		<link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
		<script type="text/javascript" src="../includes/remedial.js"></script>
		<script type="text/javascript" src="../includes/superTitle.js"></script>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		<script>

			function abreRelatorio( cod, rel ){
			
				var indNivel = 0;
				
				d = cod.retornaElemento();
				abrirTodos(d[indNivel],'', cod, rel);
			} 
		
			function abrirTodos(elemento, idPai, cod, rel){
				if( elemento['elemento'].length == 0 ) return;
				var id = elemento['id'];

				window.setTimeout("d" + rel + ".controle('" + id + "', '" + (idPai ? idPai : id) + "', '" + id + "_img')", 2);
				idPai = (idPai ? idPai + ":" + id : id);

				for( var i = 0; i < elemento['elemento'].length; i++ ){
					abrirTodos(elemento['elemento'][i], idPai, cod, rel);
				}
				
			}
		</script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	
<?php
include APPRAIZ. 'includes/classes/relatorio.class.inc';

$_SESSION['par']['postrelat'] = $_POST;

$sql = monta_sql();
$dados = $db->carregar($sql);
$bola = $dados;
$a = array();
if( is_array($dados) ){
	foreach($dados as $k => $dado){
		
		if(!in_array($dado['rgfnumprocessosape'],$a)){
			$sql = ' select par.retornasaldoprocesso(\''.$dado['rgfnumprocessosape'].'\')';
			$teste = $db->pegaUm($sql);
			if(!$teste){
				$teste = 0;
			}
			$bola[$k]['saldoglobalconta'] = $teste;
		}
		$a[$k] = $dado['rgfnumprocessosape'];
	}
}
//dbg($bola,1);
$agrup = monta_agp();
$col = monta_coluna();
$r = new montaRelatorio();
$r->setAgrupador($agrup, $bola, $agrupcol); 
$r->setColuna($col);
$r->setBrasao(false);
$r->setTotNivel(true);
$r->setEspandir(false);
$r->setMonstrarTolizadorNivel(true);
$r->setTotalizador(true);
$r->setTolizadorLinha(true);
	
	$sql = "SELECT e.entid, e.entnome, e.muncod, e.estuf 
			FROM par.entidade   e
			WHERE
				e.dutid = ".DUTID_SECRETARIO_ESTADUAL." AND
				e.entstatus = 'A' AND
				e.estuf = '".$_POST['estado']."'";
	$dadoSecret = $db->pegaLinha( $sql ); 

	$nome = $dadoSecret['entnome'];
	
	if( $_POST['estado'] == 'RS' ){
		$nome = "JOSE CLOVIS DE AZEVEDO";
	}elseif( $_POST['estado'] == 'AC' ){
		$nome = "DANIEL DE QUEIROZ SANT’ANA";
	}elseif( $_POST['estado'] == 'RN' ){
		$nome = "BETANIA LEITE RAMALHO";
	}elseif( $_POST['estado'] == 'PB' ){
		$nome = "AFONSO CELSO CALDEIRA SCOCUGLIA";
	}elseif( $_POST['estado'] == 'AP' ){
		$nome = "JOSÉ MARIA LOBATO";
	}elseif( $_POST['estado'] == 'RO' ){
		$nome = "JORGE ALBERTO ELARRAT CANTO";
	}elseif( $_POST['estado'] == 'PA' ){
		$nome = "CLAUDIO RIBEIRO";
	}
	
	echo '<center><table width="95%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug"  style="border-bottom: 1px solid;">'
				.'	<tr bgcolor="#ffffff">' 	
				.'		<td valign="top" width="50" rowspan="1"><img src="../imagens/brasao.gif" width="45" height="45" border="0"></td>'			
				.'		<td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">'				
				.'			SIMEC- Sistema Integrado do Ministério da Educação<br/>'				
				.'			MEC / SE - Secretaria Executiva <br />'
				.'			Secretário / '.$_POST['estado'].' - '.$nome.' <br />'
				.'		</td>'
				.'		<td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">'					
				.'			Impresso por: <b>' . $_SESSION['usunome'] . '</b><br/>'					
				.'			Hora da Impressão:' . date( 'd/m/Y - H:i:s' ) . '<br />'					
				.'		</td>'					
				.'	</tr><tr>'
				.'		<td colspan="3" align="center" valign="top" style="padding:0 0 10px 0;">'
				.'			<table width="100%" border="0">'
				.'				<tr>'
				.'					<td width="33%" align="left"><img src="/imagens/bandeiras/bandeira_'.$_POST['estado'].'.jpg" border="0" /><td>'
				.'					<td colspan="2" align="right"><a href="#" onclick="window.open( \'/cte/cte.php?modulo=principal/popupEmenda&acao=A&estuf='.$_POST['estado'].'\', \'Emenda\', \'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1\' );"><img src="/imagens/logo_emenda.gif" border="0" width="145" height="58" /></a><a href="#" onclick="window.open( \'/painel/painel.php?modulo=relatorio/relatorioObra&acao=A&estado=1&estuf='.$_POST['estado'].'\', \'painel\', \'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1\' );"><img src="/imagens/logo_obras.gif" border="0" width="150" height="58" /></a><a href="#" onclick="window.open( \'/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=estado&estuf='.$_POST['estado'].'\', \'painel\', \'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1\' );"><img src="/painel/novo/images/logo.png" border="0" width="150" height="58" /></a></td>'
				.'				</tr>'
				.'			</table>'
				.'		</td>'
				.'	</tr>'					
				.'</table></center>';	

	echo '
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" width="95%">
		<tr>
			<td class="subtituloesquerda" style="background:rgb(183,255,166)"> <center> Transferência Voluntária<br> <a href="javascript: abreRelatorio( d1, 1 );">Abrir / Fechar Todos</a> </center> </td>
		</tr>
	</table>
	';
echo $r->getRelatorio();

$agrupador = $_POST['agrupador'];
if( !is_array($agrupador) ){
	$agrupador = array();
}

$exibe = 3;

if( $exibe == 3 ){

	echo '<br>
			<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" width="100%">
				<tr>
					<td class="subtituloesquerda" style="background:rgb(161,210,255)"> <center> Transferências Legais e Outros Programas<br> <a href="javascript: abreRelatorio( d2, 2 );">Abrir / Fechar Todos</a> </center> </td>
				</tr>
			</table>
			';

	$sql   = monta_sql( $exibe );
	$dados = $db->carregar($sql);
	$agrupador = monta_agp( $exibe );
	$coluna   = monta_coluna( $exibe );
	$z = new montaRelatorio();
	$z->setAgrupador($agrupador, $dados); 
	$z->setColuna($coluna);
	$z->setBrasao(false);
	$z->setTotNivel(true);
	$z->setEspandir(false);
	$z->setMonstrarTolizadorNivel(true);
	$z->setTotalizador(true);
	$z->setTolizadorLinha(true);
	
	echo $z->getRelatorio();
}

$exibe = 2;

if( $exibe == 2 ){
	
	echo '<br>
			<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" width="100%">
				<tr>
					<td class="subtituloesquerda" style="background:rgb(252,255,161)"> <center> Assistência Técnica<br> <a href="javascript: abreRelatorio( d3, 3 );">Abrir / Fechar Todos</a> </center> </td>
				</tr>
			</table>
			';

	$sql   = monta_sql( $exibe );
	$dados = $db->carregar($sql);
	$agrupador = monta_agp( $exibe );
	$coluna   = monta_coluna( $exibe );
	$x = new montaRelatorio();
	$x->setAgrupador($agrupador, $dados); 
	$x->setColuna($coluna);
	$x->setBrasao(false);
	$x->setTotNivel(false);
	$x->setEspandir(false);
	$x->setMonstrarTolizadorNivel(true);
	$x->setTotalizador(true);
	$x->setTolizadorLinha(true);
	
	echo $x->getRelatorio();
}

?>
</body>
</html>

<?php 
function monta_sql( $exibe = null ){
	
	global $filtroSession;

	extract( $_POST );

	include_once APPRAIZ . '/par/classes/modelo/RelatorioAtendimentoEstado.class.inc';
	$relAtendimentoEstado = new RelatorioAtendimentoEstado( $estado );
	
	if( $exibe == 3 ){
		
		$sql = $relAtendimentoEstado->montarSQLTransferenciasLegaisOutrosProgramas();
		
	} elseif( $exibe == 2 ){ //Assistencia Tecnica

		$sql = $relAtendimentoEstado->montarSQLAssistenciaTecnica();
// 		ver($sql,d);
	} else {
	
		$sql = $relAtendimentoEstado->montarSQLTransferenciaVoluntaria();
// 		ver($sql,d);
	}
	
//	dbg($sql);
	return $sql;
}

function monta_agp( $exibe = null ){
	$agrupador = $_POST['agrupador'];
	if( !is_array($agrupador) ){
		$agrupador = array();
	}
	
	if( $exibe == 3 ){
		
		$agp = array(
					"agrupador" => array(),
					"agrupadoColuna" => array(
												"indicador",  
												"ano",  
												"ordem",  
												"periodo",
												"estado",
												"responsavel",
												"acao",
												"unidademedida",
												"detalhe",
												"valor",
												"quantidade"
											  )	  
					);
		
		$count = 1;
			$i = 0;
			
			if( $i > 1 || $i == 0 ){
				$vari = "";	
			}
			
		$novoAgrup = array(); 
		array_push($novoAgrup, 'estado');
		array_push($novoAgrup, 'acao');
		array_push($novoAgrup, 'periodo');
//		array_push($novoAgrup, 'indicador');
		array_push($novoAgrup, 'detalhe');
		if( !is_array($agrupador) ){
			$agrupador = array();
		}
		foreach ($agrupador as $val){
			array_push($novoAgrup, $val);
		}
	
		foreach ($novoAgrup as $val):
			if($count == 1){
				$var = $vari;
			} else {
				$var = "";		
			}
			switch ($val) {
				case 'acao':
					array_push($agp['agrupador'], array(
														"campo" => "acao",
												  		"label" => "$var Ação")										
										   				);				
			   		continue;
			    break;
				case 'periodo':
					array_push($agp['agrupador'], array(
														"campo" => "periodo",
												  		"label" => "$var Período")										
										   				);				
			   		continue;
			    break;
				case 'indicador':
					array_push($agp['agrupador'], array(
														"campo" => "indicador",
												  		"label" => "$var Programa")										
										   				);				
			   		continue;
			    break;
				case 'detalhe':
					array_push($agp['agrupador'], array(
														"campo" => "detalhe",
												  		"label" => "$var Detalhe")										
										   				);				
			   		continue;
			    break;
				case 'estado':
					array_push($agp['agrupador'], array(
														"campo" => "estado",
												  		"label" => "$var Estado")										
										   				);				
			   		continue;
			    break;
			    case 'subacao':
					array_push($agp['agrupador'], array(
														"campo" => "descricaosubacao1",
												 		"label" => "$var Subação")										
										   				);					
			   		continue;			
			    break;
			}
			$count++;
		endforeach;
		
	} elseif( $exibe == 2 ){
		$agp = array(
					"agrupador" => array(),
					"agrupadoColuna" => array(
												"descricaosubacao",  
												"unidademedida",  
												"planejado",  
												"aprovado",
												"emanalise",
												"qtdmonitoramento",
												"qtdpainel",
												"qtdindcadorpainel",
												"qtdoutrafonte",
												"qtdfonte",
												"programa",
												"valor"
											  )	  
					);
		
		$count = 1;
			$i = 0;
			
			if( $i > 1 || $i == 0 ){
				$vari = "";	
			}
			
		$novoAgrup = array(); 
		array_push($novoAgrup, 'estado');
		array_push($novoAgrup, 'subacao');
		if( !is_array($agrupador) ){
			$agrupador = array();
		}
		foreach ($agrupador as $val){
			array_push($novoAgrup, $val);
		}

		foreach ($novoAgrup as $val):
			if($count == 1){
				$var = $vari;
			} else {
				$var = "";		
			}
			switch ($val) {
				case 'estado':
					array_push($agp['agrupador'], array(
														"campo" => "estado",
												  		"label" => "$var Estado")										
										   				);				
			   		continue;
			    break;
			    case 'subacao':
					array_push($agp['agrupador'], array(
														"campo" => "descricaosubacao",
												 		"label" => "$var Subação")										
										   				);					
			   		continue;			
			    break;
			}
			$count++;
		endforeach;
	} else {
	
                    $agp = array(
                        "agrupador" => array(),
                        "agrupadoColuna" => array(
                            "rgfnumprocessosape",
                            "numeroconvenio",
                            "descricaosubacao",
                            "valorconvenioporsub",
                            "valorempenhado",
                            "valorpago",
                            "porcentagemexecucao",
                            "porcentagemexecucaoconv",
                            "porcentagemrepasse",
                            "saldoglobalconta",
                            "ultimadatadaconta"
                )
        );

        $count = 1;
			$i = 0;
			
			if( $i > 1 || $i == 0 ){
				$vari = "";	
			}
			
		$novoAgrup = array(); 
		array_push($novoAgrup, 'estado');
		array_push( $agrupador, 'numconvenio');
		array_push( $agrupador, 'subacao');
		array_push( $agrupador, 'vlrconvenio');
		array_push( $agrupador, 'vlrempenhado');
		array_push( $agrupador, 'vlrpago');
		array_push( $agrupador, 'porcentexec');
		array_push( $agrupador, 'saldoconta');
		array_push( $agrupador, 'dataatualizacaoconta');
		
		foreach ($agrupador as $val){
			array_push($novoAgrup, $val);
		}
	
		foreach ($novoAgrup as $val):
			if($count == 1){
				$var = $vari;
			} else {
				$var = "";		
			}
			switch ($val) {
				case 'estado':
					array_push($agp['agrupador'], array(
														"campo" => "estado",
												  		"label" => "$var Estado")										
										   				);				
			   		continue;
			    break;
				case 'numconvenio':
					array_push($agp['agrupador'], array(
														"campo" => "numeroconvenio",
												  		"label" => "$var Convênio")										
										   				);				
			   		continue;
			    break;
			    case 'descricaodimensao':
					array_push($agp['agrupador'], array(
														"campo" => "descricaodimensao",
												  		"label" => "$var Dimensão")										
										   				);					
			    	continue;
			    break;		    	
			    case 'area':
					array_push($agp['agrupador'], array(
														"campo" => "descricaoarea",
												 		"label" => "$var Área")										
										   				);					
			    	continue;			
			    break;
			    case 'indicador':
					array_push($agp['agrupador'], array(
														"campo" => "descricaoindicador",
												 		"label" => "$var Índicador")										
										   				);					
			   		continue;			
			    break;
			    case 'acao':
					array_push($agp['agrupador'], array(
														"campo" => "descricaoacao",
												 		"label" => "$var Ação")										
										   				);					
			   		continue;			
			    break;
			    case 'subacao':
					array_push($agp['agrupador'], array(
														"campo" => "descricaosubacao",
												 		"label" => "$var Subação")										
										   				);					
			   		continue;			
			    break;
			    case 'planointerno':
					array_push($agp['agrupador'], array(
														"campo" => "planointerno",
												 		"label" => "$var Plano Interno")										
										   				);					
			   		continue;			
			    break;
			    case 'programa':
					array_push($agp['agrupador'], array(
														"campo" => "programa",
												 		"label" => "$var Programa")										
										   				);					
			   		continue;			
			    break;
			}
			$count++;
		endforeach;
	}
	return $agp;
}

function monta_coluna( $exibe = null ){
	$agrupador = $_POST['agrupador'];
	if( !is_array($agrupador) ){
		$agrupador = array();
	}

	$coluna = array();
	
	if( $exibe == 3 ){
		
		array_push( $agrupador, "ano" );
//		array_push( $agrupador, "detalhe" );
		array_push( $agrupador, "valor" );
		
		
		foreach ($agrupador as $val){
			switch ($val) {
				case 'quantidade':
					array_push($coluna, array(
							  "campo" => "quantidade",
					   		  "label" => "Quantidade",
							  "type"  => "numeric"
								)
										);				
			   		continue;
			    break;
				case 'valor':
					array_push($coluna, array(
							  "campo" => "valor",
					   		  "label" => "Valor",
//							  "type"  => "numeric"
//							  "mostraNivel" => "numeroconvenio")
							  "html"  => "<b>{valor}</b>"
									)	);				
			   		continue;
			    break;
				case 'detalhe':
					array_push($coluna, array(
							  "campo" => "detalhe",
					   		  "label" => "Detalhe",
							  "type"  => "string")
										);				
			   		continue;
			    break;
				case 'unidademedida':
					array_push($coluna, array(
							  "campo" => "unidademedida",
					   		  "label" => "Unidade de Medida",
							  "type"  => "string")
										);				
			   		continue;
			    break;
				case 'acao':
					array_push($coluna, array(
							  "campo" => "acao",
					   		  "label" => "Ação",
							  "type"  => "string")
										);				
			   		continue;
			    break;
				case 'responsavel':
					array_push($coluna, array(
							  "campo" => "responsavel",
					   		  "label" => "Responsável",
							  "type"  => "string")
										);				
			   		continue;
			    break;
				case 'periodo':
					array_push($coluna, array(
							  "campo" => "periodo",
					   		  "label" => "Período",
							  "type"  => "string")
										);				
			   		continue;
			    break;
				case 'ordem':
					array_push($coluna, array(
							  "campo" => "ordem",
					   		  "label" => "Ordem",
							  "type"  => "string")
										);				
			   		continue;
			    break;
				case 'indicador':
					array_push($coluna, array(
							  "campo" => "indicador",
					   		  "label" => "Indicador",
							  "type"  => "string")
										);				
			   		continue;
			    break;
				case 'aprovado':
					array_push($coluna, array(
							  "campo" => "ano",
					   		  "label" => "Ano",
							  "type"  => "string")
										);				
			   		continue;
			    break;
			}
		}
		
	} elseif( $exibe == 2 ){
		array_push( $agrupador, "descricaosubacao" );
		array_push( $agrupador, "unidademedida" );
		array_push( $agrupador, "programa" );
		array_push( $agrupador, "planejado" );
		array_push( $agrupador, "aprovado" );
		array_push( $agrupador, "emanalise" );
		array_push( $agrupador, "qtdmonitoramento" );
		
		if( $_POST['agrupadorFiltro'] ){
			foreach( $_POST['agrupadorFiltro'] as $colunas ){
				if( $colunas == 1 ){
					array_push( $agrupador, "qtdpainel" );
				} elseif( $colunas == 2 ){
					array_push( $agrupador, "qtdindcadorpainel" );
				} elseif( $colunas == 3 ){
					array_push( $agrupador, "qtdoutrafonte" );
				} elseif( $colunas == 4 ){
					array_push( $agrupador, "qtdfonte" );
				}
			}
		}
		
		foreach ($agrupador as $val){
			switch ($val) {
				case 'valor':
					array_push($coluna, array(
							  "campo" => "valor",
					   		  "label" => "Valor",
							  "type"  => "<b>{valor}</b>")
										);				
			   		continue;
			    break;
				case 'qtdfonte':
					array_push($coluna, array(
							  "campo" => "qtdfonte",
					   		  "label" => "Fonte",
							  "type"  => "string")
										);				
			   		continue;
			    break;
				case 'programa':
					array_push($coluna, array(
							  "campo" => "programa",
					   		  "label" => "Programa",
							  "type"  => "string")
										);				
			   		continue;
			    break;
				case 'qtdoutrafonte':
					array_push($coluna, array(
							  "campo" => "qtdoutrafonte",
					   		  "label" => "Qtd. Outra Fonte",
							  "title" => "&nbsp;",
							  "type"  => "string",
							  "php"   => array("expressao" => "({qtdoutrafonte} != '0')",
											   "html"	   => "<a onmouseout=\"SuperTitleOff( this );\" onmousemove=\"SuperTitleAjax( 'cte.php?modulo=relatorio/resultRelatorioGeral&acao=A&painel=dstqtdoutrafonte', this);\">{outrafonte}</a>",
											   "type"	   => "string",	
											   "var"	   => "outrafonte",
											   "true"	   => "{qtdoutrafonte}",
											   "false"	   => "&nbsp;&nbsp;&nbsp;")
							)
										);				
			   		continue;
			    break;
				case 'qtdindcadorpainel':
					array_push($coluna, array(
							  "campo" => "qtdindcadorpainel",
					   		  "label" => "Indicador Painel",
							  "type"  => "string")
										);				
			   		continue;
			    break;
				case 'qtdpainel':
					array_push($coluna, array(
							  "campo" => "qtdpainel",
					   		  "label" => "Qtd. Painel",
							  "title" => "&nbsp;",
							  "type"  => "string",
							  "php"   => array("expressao" => "({qtdpainel} != '0')",
											   "html"	   => "<a onmouseout=\"SuperTitleOff( this );\" onmousemove=\"SuperTitleAjax( 'cte.php?modulo=relatorio/resultRelatorioGeral&acao=A&painel=dstqtdpainel', this);\">{qpainel}</a>",
											   "type"	   => "string",	
											   "var"	   => "qpainel",
											   "true"	   => "{qtdpainel}",
											   "false"	   => "&nbsp;&nbsp;&nbsp;")
												)
										);				
			   		continue;
			    break;
				case 'qtdmonitoramento':
					array_push($coluna, array(
							  "campo" => "qtdmonitoramento",
					   		  "label" => "Qtd. Monit.",
					   		  "title" => "Quantidade informada pelo ente",
							  "type"  => "string", 
							  "php"   => array("expressao" => "({qtdmonitoramento} != '0')",
											   "html"	   => "<a onmouseout=\"SuperTitleOff( this );\" onmousemove=\"SuperTitleAjax( 'cte.php?modulo=relatorio/resultRelatorioGeral&acao=A&painel=dstqtdmonitorada', this);\">{qtdmonit}</a>",
											   "type"	   => "string",	
											   "var"	   => "qtdmonit",
											   "true"	   => "{qtdmonitoramento}",
											   "false"	   => "&nbsp;&nbsp;&nbsp;")
											)
										);					
			   		continue;
			    break;
				case 'emanalise':
					array_push($coluna, array(
							  "campo" => "emanalise",
					   		  "label" => "Em Análise",
							  "title" => "&nbsp;",
							  "type"  => "numeric",
							  "php"   => array("expressao" => "({emanalise} != '0')",
											   "html"	   => "<a onmouseout=\"SuperTitleOff( this );\" onmousemove=\"SuperTitleAjax( 'cte.php?modulo=relatorio/resultRelatorioGeral&acao=A&painel=quantidadeGlobalEmanalise', this);\">{analise}</a>",
											   "type"	   => "string",	
											   "var"	   => "analise",
											   "true"	   => "{emanalise}",
											   "false"	   => "&nbsp;&nbsp;&nbsp;")
											)
										);				
			   		continue;
			    break;
				case 'aprovado':
					array_push($coluna, array(
							  "campo" => "aprovado",
					   		  "label" => "Aprovado",
							  "title" => "&nbsp;",
							  "type"  => "numeric",
							  "php"   => array("expressao" => "({aprovado} != '0')",
											   "html"	   => "<a onmouseout=\"SuperTitleOff( this );\" onmousemove=\"SuperTitleAjax( 'cte.php?modulo=relatorio/resultRelatorioGeral&acao=A&painel=quantidadeGlobalAprovado', this);\">{qaprovado}</a>",
											   "type"	   => "string",	
											   "var"	   => "qaprovado",
											   "true"	   => "{aprovado}",
											   "false"	   => "&nbsp;&nbsp;&nbsp;")
												)
										);				
			   		continue;
			    break;
				case 'planejado':
					array_push($coluna, array(
							  "campo" => "planejado",
					   		  "label" => "Planejado",
							  "title" => "&nbsp;",
							  "type"  => "numeric",
							  "php"   => array("expressao" => "({planejado} != '0')",
											   "html"	   => "<a onmouseout=\"SuperTitleOff( this );\" onmousemove=\"SuperTitleAjax( 'cte.php?modulo=relatorio/resultRelatorioGeral&acao=A&painel=quantidadeGlobalPlanejado', this);\">{qplanejado}</a>",
											   "type"	   => "string",	
											   "var"	   => "qplanejado",
											   "true"	   => "{planejado}",
											   "false"	   => "&nbsp;&nbsp;&nbsp;")
											)
										);				
			   		continue;
			    break;
				case 'unidademedida':
					array_push($coluna, array(
							  "campo" => "unidademedida",
					   		  "label" => "Unidade de Medida",
							  "type"  => "string")
										);				
			   		continue;
			    break;
			}
		}
	} else {
	
		array_push( $agrupador, 'rgfnumprocessosape');
		array_push( $agrupador, 'numconvenio');
		array_push( $agrupador, 'subacao');
		array_push( $agrupador, 'vlrconvenio');
		array_push( $agrupador, 'vlrempenhado');
		array_push( $agrupador, 'vlrpago');
		array_push( $agrupador, 'porcentvalorconv');
		array_push( $agrupador, 'saldoglobalconta');
		array_push( $agrupador, 'dataatualizacaoconta');
	
		foreach ($agrupador as $val){
			switch ($val) {		
				case 'rgfnumprocessosape':
					array_push($coluna, array(
                                            "campo" => "rgfnumprocessosape",
                                            "label" => "Processo",
                                            "type" => "string",
                                            "blockAgp" => array("descricaosubacao"),
                                            "mostraNivel" => "numeroconvenio",
                                            "html" => "<b>{rgfnumprocessosape}</b>"
                                        ));
					continue;
				break;
				case 'valorpagosubacao':
					array_push($coluna, array(
											  "campo" => "valorpagosubacao",
									   		  "label" => "Valor Pago da Subacao",
											  "html"  => "<b>{valorpagosubacao}</b>")
										);				
			   		continue;
			    break;
				case 'valorempenhadosubacao':
					array_push($coluna, array(
											  "campo" => "valorempenhadosubacao",
									   		  "label" => "Valor Empenhado da Subacao",
											  "html"  => "<b>{valorempenhadosubacao}</b>")
										);				
			   		continue;
			    break;
				case 'porcentvalorconv':
					array_push($coluna, array(
											  "campo" => "porcentagemexecucaoconv",
									   		  "label" => "Execução Convênio (%)",
											  "type"  => "numeric",
											  "blockAgp" => array("estado", "descricaosubacao"),
											  "mostraNivel" => "numeroconvenio",
											  "php"   => array("expressao" => "({valorpago} > 0 && {valorconvenioporsub} > 0)",
															   "html"	   => "{percentconv}%",
															   "type"	   => "numeric",	
															   "var"	   => "percentconv",
															   "true"	   => "number_format(({valorpago} / {valorconvenioporsub}) * 100,1,',','.')",
															   "false"	   => "0")
											)
										);				
			   		continue;
			    break;
				case 'porcentagemrepasse':
					array_push($coluna, array(
											  "campo" => "porcentagemrepasse",
									   		  "label" => "Execução Convênio (%)",
											  "type"  => "numeric",
											  "blockAgp" => array("estado", "descricaosubacao"),
											  "mostraNivel" => "numeroconvenio",
											  "php"   => array("expressao" => "({valorpago} > 0 && {saldoglobalconta} > 0)",
															   "html"	   => "{percentrepasse}%",
															   "type"	   => "numeric",	
															   "var"	   => "percentrepasse",
															   "true"	   => "str_replace('.', ',', substr(((({valorpago} - {saldoglobalconta}) / {valorpago}) * 100), 0, 2 + (strpos((({valorpago} - {saldoglobalconta}) / {valorpago}) * 100, '.'))))",
															   "false"	   => "0")
											)
										);				
			   		continue;
			    break;
				case 'porcentexec':
					array_push($coluna, array(
											  "campo" => "porcentagemexecucao",
									   		  "label" => "Execução (%)",
											  "type"  => "numeric")
										);				
			   		continue;
			    break;
				case 'dimensao':
					array_push($coluna, array(
											  "campo" => "descricaodimensao",
									   		  "label" => "Dimensão",
											  "type"  => "string")
										);				
			   		continue;
			    break;
				case 'area':
					array_push($coluna, array(
											  "campo" => "descricaoarea",
									   		  "label" => "Área",
											  "type"  => "string")
										);				
			   		continue;
			    break;
				case 'indicador':
					array_push($coluna, array(
											  "campo" => "descricaoindicador",
									   		  "label" => "Indicador",
											  "type"  => "string")
										);				
			   		continue;
			    break;
				case 'acao':
					array_push($coluna, array(
											  "campo" => "descricaoacao",
									   		  "label" => "Ação",
											  "type"  => "string")
										);				
			   		continue;
			    break;
				case 'dataatualizacaoconta':
					array_push($coluna, array(
											  "campo" => "ultimadatadaconta",
									   		  "label" => "Data da Última Atualização da Conta",
											  "type"  => "string")
										);				
			   		continue;
			    break;
				case 'saldoaplicacao':
					array_push($coluna, array(
											  "campo" => "saldoglobalaplicacao",
									   		  "label" => "Saldo Global da Aplicação",
											  "html"  => "<b>{saldoglobalaplicacao}</b>")
										);				
			   		continue;
			    break;
				case 'saldoglobalconta':
					array_push($coluna, array(
											"campo" => "saldoglobalconta",
											"label" => "Saldo Global da Conta",
											"type" => "string",
											"blockAgp" => array("descricaosubacao"),
											"mostraNivel" => array("numeroconvenio"),
											"html" => "<b style='color: rgb(0, 102, 204);'>{saldoformatado}</b>",
											"php" => array("expressao" => "is_numeric({saldoglobalconta})",
	                                                    	"type" => "numeric",
	                                                    	"var" => "saldoformatado",
	                                                    	"true" => "number_format({saldoglobalconta},2,',','.')",
	                                                    	"false" => "{saldoglobalconta}") 
											)
					);
					continue;
					break;
				case 'vlrproponente':
					array_push($coluna, array(
										  	"campo" => "valorglobalproponente",
								   		  	"label" => "Valor Global do Proponente",
										  	"html"  => "<b>{valorglobalproponente}</b>")
										);				
			   		continue;
			    break;
				case 'vlrconcedente':
					array_push($coluna, array(
											  "campo" => "valorglobalconcedente",
									   		  "label" => "Valor Global do Concedente",
											  "html"  => "<b>{valorglobalconcedente}</b>")
										);				
			   		continue;
			    break;
				case 'vlraempenhar':
					array_push($coluna, array(
											  "campo" => "valorglobalaempenhar",
									   		  "label" => "Valor Global a Empenhar",
											  "html"  => "<b>{valorglobalaempenhar}</b>")
										);				
			   		continue;
			    break;
				case 'vlrapagar':
					array_push($coluna, array(
											  "campo" => "valorglobalapagar",
									   		  "label" => "Valor Global a Pagar",
											  "html"  => "<b>{valorglobalapagar}</b>")
										);				
			   		continue;
			    break;
				case 'vlrpago':
					array_push($coluna, array(
											  "campo" => "valorpago",
									   		  "label" => "Valor Pago",
											  "html"  => "<b>{valorpago}</b>")
										);				
			   		continue;
			    break;
				case 'vlrempenhado':
					array_push($coluna, array(
											  "campo" => "valorempenhado",
									   		  "label" => "Valor Empenhado",
											  "html"  => "<b>{valorempenhado}</b>")
										);				
			   		continue;
			    break;
				case 'vlrconvenio':
			   		array_push($coluna, array(
											  "campo" => "valorconvenioporsub",
									   		  "label" => "Valor do Convênio",
											  "blockAgp" => "descricaosubacao",
											  "mostraNivel" => "numeroconvenio",	
											  "html"  => "<b style='color: rgb(0, 102, 204);'>{valorformatado}</b>",
											  "php"   => array("expressao" => "is_numeric({valorconvenioporsub})",
															   "type"	   => "numeric",	
															   "var"	   => "valorformatado",
															   "true"	   => "number_format({valorconvenioporsub},2,',','.')",
															   "false"	   => "{valorconvenioporsub}")
											 )
										);				
			   		continue;
			    break;
				case 'planointerno':
					array_push($coluna, array(
											  "campo" => "planointerno",
									   		  "label" => "Plano Interno",
											  "type"  => "string")
										);				
			   		continue;
			    break;
				case 'programa':
					array_push($coluna, array(
											  "campo" => "programa",
									   		  "label" => "Programa",
											  "type"  => "string")
										);				
			   		continue;
			    break;
			}
		}
	}
	return $coluna;	
}
?>
<script>
jQuery.noConflict();
var arTabelas = jQuery('table');
arTabelas[3].style.background = 'rgb(220,255,220)';
arTabelas[5].style.background = 'rgb(220,240,255)';
arTabelas[7].style.background = 'rgb(255,255,220)';

</script>