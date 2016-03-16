<?php
/*
if( (date("H") >= 8 && date("H") <= 11) || (date("H") >= 14 && date("H") <= 17) ){
	echo '<center>
		 <h3>
		 EM MANUTENÇÃO...
		 <br><br>HORÁRIO PERMITIDO: 00:00 AS 07:59 / 12:00 AS 13:59 / 18:00 AS 23:59
		 </h3>
		 <br><br>
		 <input type="button" name="Fechar" value="Fechar" onclick="self.close();">
		 </center>';
	
	die();
}
*/

ini_set("memory_limit", "2048M");
set_time_limit(80000);
ini_set('max_execution_time', 300);

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
		location.href = '?modulo=relatorio/relatorio_os&acao=A';
	</script>
	<?
	die;
}

//recupera perfil do usuário
$perfil = arrayPerfil();


//coloca periodo e valor do ponto no titulo
if(!$_REQUEST['titulo']) $_REQUEST['titulo'] = "Relatório de Demandas";

/*
if ( in_array(DEMANDA_PERFIL_SUPERUSUARIO, $perfil) || in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil) || in_array(DEMANDA_PERFIL_ANALISTA_SISTEMA, $perfil) || in_array(DEMANDA_PERFIL_GESTOR_REDES, $perfil) || in_array(DEMANDA_PERFIL_DBA, $perfil) || in_array(DEMANDA_PERFIL_ADMINISTRADOR, $perfil) || in_array(DEMANDA_PERFIL_ANALISTA_WEB, $perfil) || in_array(DEMANDA_PERFIL_GESTOR_MEC, $perfil) || in_array(DEMANDA_PERFIL_ANALISTA_TECNICO, $perfil) ){
	$valorpto = $db->PegaUm("select crtvlponto from demandas.contrato where crtstatus='A' and (current_timestamp between crtdtinicio and crtdtfim) limit 1");
	if($valorpto){
		$valorpto = "<br><font style='font-size:10px;'>Valor do Ponto: R$ ".number_format($valorpto,2,",",".")."</font>";
	}
	else{
		$valorpto = "<br><font style='font-size:10px;'>Valor do Ponto: R$ 0,00</font>";
	}
}
*/

$txtdtinifim = "";
if( $_REQUEST['dtinicio'] && $_REQUEST['dtfim'] ) $txtdtinifim = "<br><font style='font-size:10px;'>Período de Abertura: ".$_REQUEST['dtinicio']." à ".$_REQUEST['dtfim']."</font>";

$txtdtinifimsit = "";
if( $_REQUEST['dtinisit'] && $_REQUEST['dtfimsit'] ) $txtdtinifimsit = "<br><font style='font-size:10px;'>Período da Situação: ".$_REQUEST['dtinisit']." à ".$_REQUEST['dtfimsit']."</font>";

//$_REQUEST['titulo'] = $_REQUEST['titulo'] . $valorpto . $txtdtinifim . $txtdtinifimsit;
$_REQUEST['titulo'] = $_REQUEST['titulo'] . $txtdtinifim . $txtdtinifimsit;


 

// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';
include_once APPRAIZ . 'includes/workflow.php';

$classdata = new Data;

$sql   = monta_sql();

//echo "<pre>";print_r( $sql);exit;

$dados = $db->carregar($sql);

$total_minuto = 0;
$total_minuto_conclusao = 0;

$colunax = (array) $_REQUEST['coluna'];

if ( in_array("prazoatendimento", $colunax) || in_array("tempodecorrido", $colunax) || in_array("duracaoatendminutos", $colunax) ||
	 in_array("tempopausa", $colunax) || in_array("tempoclassif", $colunax) || in_array("tempoclassifsi", $colunax) || in_array("nseriepatr", $colunax) ||
	 in_array("observacao", $colunax) || in_array("totalpontuacao", $colunax) || in_array("qtdhoras", $colunax) ||
	 in_array("valordemanda", $colunax) || in_array("histtramite", $colunax)){
	 
 
	$valorzero = 0;

	$keyzero = false;
	
	for($i = 0; $i<=count($dados); $i++ ){
	
		if( $dados[$i]['dmddatainiprevatendimento'] != ''   &&
		   	$dados[$i]['dmddatafimprevatendimento'] != ''	&&
		   	(in_array("prazoatendimento", $colunax) 		|| 
		   	in_array("tempodecorrido", $colunax)			||
		   	in_array("duracaoatendminutos", $colunax)		||
		   	in_array("tempopausa", $colunax))
		   ){
		   
		   		//calcula prazo previsto 
	   			$total_minuto = calculaTempoMinuto($dados[$i]['dmddatainiprevatendimento'], $dados[$i]['dmddatafimprevatendimento'], $dados[$i]['dmdhorarioatendimento'], $dados[$i]['ordid']);
	   			//if($dados[$i]['qtdservico']) $total_minuto = $total_minuto * (int) $dados[$i]['qtdservico'];
	   			
				//verifica pausa da demanda
				$sql = "select t.tpadsc, p.pdmdatainiciopausa, p.pdmdatafimpausa, p.pdmjustificativa, to_char(p.pdmdatainiciopausa::timestamp,'DD/MM/YYYY HH24:MI') AS datapausaini, to_char(p.pdmdatafimpausa::timestamp,'DD/MM/YYYY HH24:MI') AS datapausafim
						from demandas.pausademanda p 
						inner join demandas.tipopausademanda t ON t.tpaid = p.tpaid
						where p.dmdid = ". (int) $dados[$i]['nudemanda'];
						
				$dadosp = $db->carregar($sql);	
				
				$flagIndeterminado = '';
				$tempototalpausa = 0;
				$textotempopausa = "<div align='left'>";
				$horasx = 0;
				$minutosx = 0;
				
				if($dadosp){
					foreach($dadosp as $dadop){
						
						if($dadop['pdmdatainiciopausa'] && $dadop['pdmdatafimpausa']){
							
							$ano_inip	= substr($dadop['pdmdatainiciopausa'],0,4);
							$mes_inip	= substr($dadop['pdmdatainiciopausa'],5,2);
							$dia_inip	= substr($dadop['pdmdatainiciopausa'],8,2);
							$hor_inip	= substr($dadop['pdmdatainiciopausa'],11,2);
							$min_inip	= substr($dadop['pdmdatainiciopausa'],14,2);
				
							$ano_fimp	= substr($dadop['pdmdatafimpausa'],0,4);
							$mes_fimp	= substr($dadop['pdmdatafimpausa'],5,2);
							$dia_fimp	= substr($dadop['pdmdatafimpausa'],8,2);
							$hor_fimp	= substr($dadop['pdmdatafimpausa'],11,2);
							$min_fimp	= substr($dadop['pdmdatafimpausa'],14,2);
							
							$dinip = mktime($hor_inip,$min_inip,0,$mes_inip,$dia_inip,$ano_inip); // timestamp da data inicial
							$dfimp = mktime($hor_fimp,$min_fimp,0,$mes_fimp,$dia_fimp,$ano_fimp); // timestamp da data final
							
							// pega o tempo total da pausa
							$tempototalpausa = $tempototalpausa + ($dfimp - $dinip);
							
							
							$dtiniinvert = $ano_inip.'-'.$mes_inip.'-'.$dia_inip.' '.$hor_inip.':'.$min_inip.':00';
							$dtfiminvert = $ano_fimp.'-'.$mes_fimp.'-'.$dia_fimp.' '.$hor_fimp.':'.$min_fimp.':00';
							
						}
	
						//monta o texto da tempopausa
						$textotempopausa .= "<b>Tipo:</b> ". $dadop['tpadsc'];
						$textotempopausa .= "<br><b>Justificativa:</b> ". $dadop['pdmjustificativa']."";
						$textotempopausa .= "<br><b>Data início:</b> ". $dadop['datapausaini']."";
						if($dadop['datapausafim']){
							$textotempopausa .= "<br><b>Data término:</b> ". $dadop['datapausafim']."";
						}else{
							$textotempopausa .= "<br><b>Data término:</b> Indeterminado";
						}
						
						if($dadop['pdmdatafimpausa']){
							$tempop = $classdata->diferencaEntreDatas(  $dtiniinvert, $dtfiminvert, 'tempoEntreDadas', 'string','yyyy/mm/dd');
							if(!$tempop) $tempop = '0 minuto';
							$textotempopausa .= "<br><b>Tempo da Pausa:</b> ".$tempop;
						}else{
							$flagIndeterminado = ' + <font color=red>Tempo Indeterminado</font>';
							$textotempopausa .= "<br><b>Tempo da Pausa:</b> Indeterminado";
						}
						
						$textotempopausa .= "<BR><BR>";
						
					}
					
						$datainiaux = date('Y-m-d H:i').':00';
						$ano_aux	= substr($datainiaux,0,4);
						$mes_aux	= substr($datainiaux,5,2);
						$dia_aux	= substr($datainiaux,8,2);
						$hor_aux	= substr($datainiaux,11,2);
						$min_aux	= substr($datainiaux,14,2);
						
						$datafinalaux = mktime($hor_aux,$min_aux,0+$tempototalpausa,$mes_aux,$dia_aux,$ano_aux);
						$datafinalaux2 = strftime("%Y-%m-%d %H:%M:%S", $datafinalaux);
						$tempototalp = $classdata->diferencaEntreDatas(  $datainiaux, $datafinalaux2, 'tempoEntreDadas', 'string','yyyy/mm/dd');
						$textotempopausa .= "<b>TOTAL (Tempo da Pausa):</b> ". $tempototalp . $flagIndeterminado;
						
					//pega prioridade e data termino
					$sql = "select dmdhorarioatendimento as dmdhorarioatendimentop, to_char(dmddatafimprevatendimento::timestamp,'DD/MM/YYYY HH24:MI') AS dmddatafimprevatendimentop
							from demandas.demanda 
							where dmdid = ". (int) $dados[$i]['nudemanda'];
					$dadosdmd = $db->carregar($sql);	
					
					$resto = $tempototalpausa;
					$horas 			= $resto/3600; //quantidade de horas
			        $intHoras 		= floor($horas);
			        if($intHoras >= 1){	//se houver horas
			            $horasx = $intHoras;
			            $resto 		 = $resto-($intHoras*3600); //retira do total, o tempo em segundos das horas passados
			        }
			                    
			        $minutos 		= $resto/60; //quantidade de minutos
			        $intMinutos 	= floor($minutos);
			        if($intMinutos >= 1){ //se houver minutos
			            $minutosx = $intMinutos;
			            $resto 		 = $resto-($intMinutos*60); //retira do total, o tempo em segundos dos minutos passados
			        }				
			        
		            if(!$horasx) $horasx = "00";
		            if(strlen($horasx) == 1) $horasx = "0".$horasx;
		            if(!$minutosx) $minutosx = "00";
		            if(strlen($minutosx) == 1) $minutosx = "0".$minutosx;
		            
		            $hormin = $horasx.":".$minutosx;
					
					$vfdtfim = verificaCalculoTempoDtfim($dadosdmd[0]['dmddatafimprevatendimentop'], $hormin, $dadosdmd[0]['dmdhorarioatendimentop'], $dados[$i]['dataconclusao'], $dados[$i]['ordid']);
					
					if($flagIndeterminado){
						$textotempopausa .= "<br><br><b>Data Prevista de Término:</b> <font color=red>Data Indeterminada</font>";
					}
					else{
						$textotempopausa .= "<br><br><b>Data Prevista de Término:</b> <font color=black>".$vfdtfim."</font>";
						//$textotempopausa .= "<br><br><b>Data Prevista de Término:</b> <font color=black>".$horasx.":".$minutosx."</font>";
					}
						
				}
				
				$textotempopausa .= "</div>"; 

				//atribui o campo tempo da pausa
				$dados[$i]['tempopausa'] = $textotempopausa;
				
				$ano_ini	= substr($dados[$i]['dmddatainiprevatendimento'],0,4);
				$mes_ini	= substr($dados[$i]['dmddatainiprevatendimento'],5,2);
				$dia_ini	= substr($dados[$i]['dmddatainiprevatendimento'],8,2);
				$hor_ini	= substr($dados[$i]['dmddatainiprevatendimento'],11,2);
				$min_ini	= substr($dados[$i]['dmddatainiprevatendimento'],14,2);
				
				//verifica se a situação é 'Validada Fora do Prazo' se sim, despreza o tempo da pausa
				if($dados[$i]['esdid'] == DEMANDA_ESTADO_VALIDADA_FORA_PRAZO) $tempototalpausa = 0; 
				
				
				$dataFinal = mktime($hor_ini,$min_ini+$total_minuto,0+$tempototalpausa,$mes_ini,$dia_ini,$ano_ini); // timestamp da data final
				$dataFinalPrazoPrev = strftime("%Y-%m-%d %H:%M:%S", $dataFinal);			
				
				$dados[$i]['prazoatendimento'] = $classdata->diferencaEntreDatas(  $dados[$i]['dmddatainiprevatendimento'], $dataFinalPrazoPrev , 'tempoEntreDadas', 'string','yyyy/mm/dd');
				if(!$dados[$i]['prazoatendimento']) $dados[$i]['prazoatendimento'] = "0 minuto";
				
				if($dados[$i]['datadocfinalizada']){
				
					//calcula Duração do atendimento
					$total_minuto_conclusao = calculaTempoMinuto($dados[$i]['dmddatainiprevatendimento'], $dados[$i]['datadocfinalizada'], $dados[$i]['dmdhorarioatendimento'], $dados[$i]['ordid']);
					$dataFinalConc = mktime($hor_ini,$min_ini+$total_minuto_conclusao,0,$mes_ini,$dia_ini,$ano_ini); 
					$dataFinalConclusao = strftime("%Y-%m-%d %H:%M:%S", $dataFinalConc);			
					
					$total_prazoatendimento = (float) ( str_replace(':','',str_replace(' ','',str_replace('-','',$dataFinalPrazoPrev))) - str_replace(':','',str_replace(' ','',str_replace('-','',$dados[$i]['dmddatainiprevatendimento']))) );
					$total_tempodecorrido = (float) ( str_replace(':','',str_replace(' ','',str_replace('-','',$dataFinalConclusao))) - str_replace(':','',str_replace(' ','',str_replace('-','',$dados[$i]['dmddatainiprevatendimento']))) );
					
					$dados[$i]['tempodecorrido'] = $classdata->diferencaEntreDatas(  $dados[$i]['dmddatainiprevatendimento'], $dataFinalConclusao , 'tempoEntreDadas', 'string','yyyy/mm/dd');
					$dados[$i]['duracaoatendminutos'] = $total_minuto_conclusao;
							
					
					if($total_tempodecorrido > $total_prazoatendimento){
						$dados[$i]['tempodecorrido'] = "<font color=red>". $dados[$i]['tempodecorrido'] . "</font>";
						$dados[$i]['dataconclusao'] = "<font color=red>". $dados[$i]['dataconclusao'] . "</font>";
					}
					else{
						$dados[$i]['tempodecorrido'] = "<font color=blue>". $dados[$i]['tempodecorrido'] . "</font>";
						$dados[$i]['dataconclusao'] = "<font color=blue>". $dados[$i]['dataconclusao'] . "</font>";
					}	
				}		
		}
		
		
		//pega o tempo da 1º classificação da demanda
		if ( in_array("tempoclassif", $colunax) ){	
			if($dados[$i]['dataabertura'] && $dados[$i]['dataclassificacao']){
				$dtabertura = $dados[$i]['dataabertura'];
				
				$hor_tc	= 0;
				$min_tc	= 15;
				
				$ano_ini	= substr($dtabertura,6,4);
				$mes_ini	= substr($dtabertura,3,2);
				$dia_ini	= substr($dtabertura,0,2);
				$hor_ini	= substr($dtabertura,11,2);
				$min_ini	= substr($dtabertura,14,2);
				$dataini = mktime($hor_ini+$hor_tc,$min_ini+$min_tc,0,$mes_ini,$dia_ini,$ano_ini); // timestamp da data final
				$datainiFinal = strftime("%Y-%m-%d %H:%M:%S", $dataini);
				$dtxini = (float) str_replace(':','',str_replace(' ','',str_replace('-','',$datainiFinal)));
				$dtxfim = (float) str_replace(':','',str_replace(' ','',str_replace('-','',$dados[$i]['dataclassificacao'])));
				$dtabertura = substr($dtabertura,6,4) ."-". substr($dtabertura,3,2) ."-". substr($dtabertura,0,2) ." ". substr($dtabertura,11,2) .":". substr($dtabertura,14,2) . ":00";
				
				if($dtxfim > $dtxini){
					$dados[$i]['tempoclassif'] = "<font color=red>".$classdata->diferencaEntreDatas(  $dtabertura , $dados[$i]['dataclassificacao'] , 'tempoEntreDadas', 'string','yyyy/mm/dd'). "</font>";
				}
				else{
					$dados[$i]['tempoclassif'] = "<font color=blue>".$classdata->diferencaEntreDatas(  $dtabertura , $dados[$i]['dataclassificacao'] , 'tempoEntreDadas', 'string','yyyy/mm/dd'). "</font>";
				}
			}
		}		

		//pega o tempo da 2º classificação da origem: serviço de impressão
		if ( in_array("tempoclassifsi", $colunax) ){	
			if($dados[$i]['dataclassificacao'] && $dados[$i]['dataclassificacaosi']){
				$dtabertura = $dados[$i]['dataclassificacao'];
				$hor_tc	= substr($dados[$i]['tempoclassifcatalogo'],0,2);
				$min_tc	= substr($dados[$i]['tempoclassifcatalogo'],3,2);
				$ano_ini	= substr($dtabertura,0,4);
				$mes_ini	= substr($dtabertura,5,2);
				$dia_ini	= substr($dtabertura,8,2);
				$hor_ini	= substr($dtabertura,11,2);
				$min_ini	= substr($dtabertura,14,2);
				$dataini = mktime($hor_ini+$hor_tc,$min_ini+$min_tc,0,$mes_ini,$dia_ini,$ano_ini); // timestamp da data final
				$datainiFinal = strftime("%Y-%m-%d %H:%M:%S", $dataini);
				$dtxini = (float) str_replace(':','',str_replace(' ','',str_replace('-','',$datainiFinal)));
				$dtxfim = (float) str_replace(':','',str_replace(' ','',str_replace('-','',$dados[$i]['dataclassificacaosi'])));
				$dtabertura = substr($dtabertura,0,4) ."-". substr($dtabertura,5,2) ."-". substr($dtabertura,8,2) ." ". substr($dtabertura,11,2) .":". substr($dtabertura,14,2) . ":00";
				
				if($dtxfim > $dtxini){
					$dados[$i]['tempoclassifsi'] = "<font color=red>".$classdata->diferencaEntreDatas(  $dtabertura , $dados[$i]['dataclassificacaosi'] , 'tempoEntreDadas', 'string','yyyy/mm/dd'). "</font>";
				}
				else{
					$dados[$i]['tempoclassifsi'] = "<font color=blue>".$classdata->diferencaEntreDatas(  $dtabertura , $dados[$i]['dataclassificacaosi'] , 'tempoEntreDadas', 'string','yyyy/mm/dd'). "</font>";
				}
			}
		}		
		
		//pega o calculo total de pontuação
		if($dados[$i]['qtdservico'] && $dados[$i]['pontuacao']){
			$dados[$i]['totalpontuacao'] = (int) $dados[$i]['qtdservico'] * (int) $dados[$i]['pontuacao']; 
		}
			
		//pega o calculo valor da demanda
		if($dados[$i]['totalpontuacao']){
			//echo $dados[$i]['totalpontuacao'] . " * ". $valordemanda."<br>";

			//pega o valor da demanda
			if($dados[$i]['ordid']){
				$sqlvl = "select crtvlponto from demandas.contrato where crtstatus='A' and ordid = ".$dados[$i]['ordid']." and ('".$dados[$i]['dmddatainclusao']."' between crtdtinicio and crtdtfim) limit 1";
				$valordemanda = $db->PegaUm($sqlvl);
			}	
			if(!$valordemanda) $valordemanda = 0;
			
			$dados[$i]['valordemanda'] = (int) $dados[$i]['totalpontuacao'] * (float) $valordemanda;
		}
	
		//pega observação
		if ( in_array("observacao", $colunax) ){
			if($dados[$i]['nudemanda']){
				$sqlob = "select u.usunome, obsdsc, to_char(obsdata::timestamp,'DD/MM/YYYY HH24:MI') AS obsdata 
						  from demandas.observacoes o
						  inner join seguranca.usuario u on u.usucpf = o.usucpf 
						  where obslog IS NULL and obsstatus='A' and dmdid = ".$dados[$i]['nudemanda'];
				$ob = array();
				$ob = $db->carregar( $sqlob );
				if($ob){
					$dados[$i]['observacao'] = '<div align=left>';
					foreach($ob as $obx){
						$dados[$i]['observacao'] .= $obx['obsdsc'] .'<br><b>Incluído por: '. $obx['usunome'] . '<br> Data: '. $obx['obsdata'] .'</b><br><br>';	
					}
					$dados[$i]['observacao'] .= '</div>';
				}	
			}
		}
		
		//pega qtdhoras
		if ( in_array("qtdhoras", $colunax) ){
			if($dados[$i]['nudemanda']){
				if(in_array("tecnico", $colunax)){
					$sqlob = "SELECT u.usunome , rh.cpftecnicoresponsavel, to_char(sum(justify_hours(cast(to_char(rh.qtdhoras,'HH24:MI') as interval))),'HH24:MI') as horas 
					FROM demandas.demanda de  
					INNER JOIN demandas.registroshorasdeatendimento rh ON de.dmdid = rh.dmdid 
					INNER JOIN seguranca.usuario u ON u.usucpf = rh.cpftecnicoresponsavel
					WHERE de.dmdid = ".$dados[$i]['nudemanda']." GROUP BY rh.cpftecnicoresponsavel, u.usunome";
				} else {
					$sqlob = "SELECT to_char(sum(justify_hours(cast(to_char(qtdhoras,'HH24:MI') as interval))),'HH24:MI') as horas 
					FROM demandas.registroshorasdeatendimento
					WHERE dmdid = ".$dados[$i]['nudemanda']."";
				}
				$ob = array();
				$ob = $db->carregar( $sqlob );
				if($ob){
					$dados[$i]['qtdhoras'] = '<div align=left>';
					foreach($ob as $obx){
						if(in_array("tecnico", $colunax)){
							$dados[$i]['qtdhoras'] .= $obx['usunome'] .' - <b>'. $obx['horas'] . '</b><br><br>';	
						} else { 
							$dados[$i]['qtdhoras'] .= '<div align=center><b>'. $obx['horas'] . '</b></div><br><br>';
						}
					}
					$dados[$i]['qtdhoras'] .= '</div>';
				}	
			}
		}		
		

		//pega nº serie / patrimonio
		if ( in_array("nseriepatr", $colunax) ){
			if($dados[$i]['nudemanda']){
				$sqln = "select ihdnumserie, ihdnumpatrimonio 
						  from demandas.itemhardwaredemanda 
						  where dmdid = ".$dados[$i]['nudemanda'];
				$nsp = array();
				$nsp = $db->carregar( $sqln );
				$ctnsp = 1;
				if($nsp){
					$dados[$i]['nseriepatr'] = '<div align=left>';
					foreach($nsp as $nspx){
						$dados[$i]['nseriepatr'] .= '<b><u>Item '.$ctnsp .'</u></b>';
						if($nspx['ihdnumserie']) $dados[$i]['nseriepatr'] .= '<br>Nº Série: '. $nspx['ihdnumserie'];
						if($nspx['ihdnumpatrimonio']) $dados[$i]['nseriepatr'] .= '<br>Nº Patrimônio: '. $nspx['ihdnumpatrimonio'];
						
						$dados[$i]['nseriepatr'] .= '<br><br>';
						
						$ctnsp++;	
					}
					$dados[$i]['nseriepatr'] .= '</div>';
				}	
			}
		}
		
		//pega historico de tramites
		/*
		if ( in_array("histtramite", $colunax) ){
			if($dados[$i]['nudemanda'] && $dados[$i]['docid']){
				
				// Busca historico
			    $docid           = (integer) $dados[$i]['docid'];
			    $dadoEstadoAtual = wf_pegarEstadoAtual( $docid );
			    $dadoHistorico   = wf_pegarHistorico( $docid );
			    
				if($dadoHistorico){
					$a = 0;
					$dados[$i]['histtramite'] = '<div align=left>';
					
					foreach($dadoHistorico as $val){
						$dados[$i]['histtramite'] .= '<b>Seq.: '. ($a + 1) . '<br>Onde Estava: '. $val['esddsc'] .'<br>O que aconteceu: '. $val['aeddscrealizada'] .'<br>Quem fez: '. $val['usunome'] .'<br>Quando fez: '. $val['htddata'];

						if($val['cmddsc']){
							$dados[$i]['histtramite'] .= '<br>Justificativa: '. $val['cmddsc'];
						}
						
						$dados[$i]['histtramite'] .= '</b><br><br>';
					}
					
					$dados[$i]['histtramite'] .= '<b>Estado atual: '.str_to_upper($dadoEstadoAtual['esddsc']).'</b><br><br><br>';
					
					$dados[$i]['histtramite'] .= '</div>';
				}
			}
			else{
				$dados[$i]['histtramite'] = '<div align=left><b>Em Processamento</b></div>';
			}
		}
		*/
		
		//elimina as demandas em atraso
		$dtatraso1 = 0;
		$dtatraso2 = 0;
		if($_REQUEST['demandaematraso']){
			if($dados[$i]['dmddatafimprevatendimento']){
				$dtatraso1 =  ( str_replace(':','',str_replace(' ','',str_replace('-','',$dados[$i]['dmddatafimprevatendimento']))) );
				$dtatraso2 =  ( str_replace(':','',str_replace(' ','',str_replace('-','',$datafinalaux2))) );
				//echo "<br>".$tempototalpausa.' - '.$dados[$i]['nudemanda'] ." = ". $dtatraso1 ." > ". $dtatraso2;
				
				if($tempototalpausa>0 && $dtatraso1 > $dtatraso2){
					unset($dados[$i]);
					if ($i==0) $keyzero = true;
				}
			}
		}
		
		
		
	}
	
	 //reordena array
	if($keyzero == true){
		$dados[0] = $dados[1];
		unset($dados[1]);
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

	$perfil = arrayPerfil();
	
	$where = array();
	
//	if ( in_array(DEMANDA_PERFIL_SUPERUSUARIO, $perfil) || in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil) || in_array(DEMANDA_PERFIL_ANALISTA_SISTEMA, $perfil) || in_array(DEMANDA_PERFIL_PROGRAMADOR, $perfil) ){	
//		
//		// origem
//		if( $origemd[0] && $origemd_campo_flag != '' ){
//			array_push($where, " od.ordid " . (!$origemd_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $origemd ) . "') ");
//		}
//		
//		// celula
//		if( $celula[0] && $celula_campo_flag != '' ){
//			array_push($where, " d.celid " . (!$celula_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $celula ) . "') ");
//		}
//		
//	}
//	else{
//	
//		// origem
//		if ( in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil) || in_array(DEMANDA_PERFIL_ANALISTA_SISTEMA, $perfil) || in_array(DEMANDA_PERFIL_PROGRAMADOR, $perfil) ){
//			array_push($where, " od.ordid IN (1)");
//		}else{
//			if( $origemd[0] && $origemd_campo_flag != '' ){
//				array_push($where, " od.ordid " . (!$origemd_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $origemd ) . "') ");
//			}
//		}
//		
//		// celula
//		if ( in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil) || in_array(DEMANDA_PERFIL_ANALISTA_SISTEMA, $perfil) || in_array(DEMANDA_PERFIL_PROGRAMADOR, $perfil) ){
//			$celidx = $db->PegaUm(" SELECT distinct celid from demandas.usuarioresponsabilidade where usucpf = '".$_SESSION['usucpf']."' and not celid is null");
//			array_push($where, " smc.celid IN ($celidx)");
//		}else{
//			if( $celula[0] && $celula_campo_flag != '' ){
//				array_push($where, " smc.celid " . (!$celula_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $celula ) . "') ");
//			}
//		}
//		
//	}
	
	// origem
	if( $origemd[0] && $origemd_campo_flag != '' ){
		array_push($where, " od.ordid " . (!$origemd_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $origemd ) . "') ");
	}
	
	// celula
	if( $celula[0] && $celula_campo_flag != '' ){
		array_push($where, " d.celid " . (!$celula_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $celula ) . "') ");
	}
			
	// Sistema
	if($sidid[0] != ''){
		array_push($where, " d.sidid " . (!$sidid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $sidid ) . "') ");
	}
		
	// Setor
	if( $setor[0] && $setor_campo_flag != '' ){
		array_push($where, " uni.unaid " . (!$setor_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode( ',', $setor ) . ") ");
	}

	// Classificação da demanda
	if( $classifdemanda[0]  && $classifdemanda_campo_flag != '' ){
		array_push($where, " d.dmdclassificacao " . (!$classifdemanda_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $classifdemanda ) . "') ");
	}
	
	// Solicitante
	if( $solicitante[0]  && $solicitante_campo_flag != '' ){
		array_push($where, " d.usucpfdemandante " . (!$solicitante_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $solicitante ) . "') ");
	}
	
	// Técnico
	if( $tecnico[0] && $tecnico_campo_flag != '' ){
		array_push($where, " d.usucpfexecutor " . (!$tecnico_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $tecnico ) . "') ");
	}

	// Gestor
	if( $gestor[0] && $gestor_campo_flag != '' ){
		array_push($where, " a2.usucpf " . (!$gestor_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $gestor ) . "') ");
	}

	// Tecnico classificador
	if( $tecnicoclassif[0] && $tecnicoclassif_campo_flag != '' ){
		//array_push($where, " usucpftecnicoclassif " . (!$tecnicoclassif_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $tecnicoclassif ) . "') ");
		array_push($where, " d.usucpfclassificador " . (!$tecnicoclassif_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $tecnicoclassif ) . "') ");
	}
	
	// prioridade
	if( $prioridade[0] && $prioridade_campo_flag != '' ){
		array_push($where, " d.priid " . (!$prioridade_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode( ',', $prioridade ) . ") ");
	}	
	
	// situacao da demanda
	if( $situacao[0] && $situacao_campo_flag != '' ){
		array_push($where, " ed.esdid " . (!$situacao_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode( ',', $situacao ) . ") ");
	}	

	// situacao da avaliacao
	if ( in_array("avaliacaog", $_REQUEST['agrupador']) || in_array("avaliacao", $_REQUEST['coluna']) ){
		
			$campoAvaliacao = "'<div align=left><b>Data: ' || to_char(avdf.avddata::TIMESTAMP,'DD/MM/YYYY HH24:MI:SS')
                                || '<br> Avaliação:</b> ' ||
                                CASE avdf.avdgeral
                                    WHEN '1' THEN 'RUIM'
                                    WHEN '2' THEN 'REGULAR'
                                    WHEN '3' THEN 'BOM'
                                    WHEN '4' THEN 'ÓTIMO'
                                END
                                ||
                                CASE
                                    WHEN avsobs <> '' THEN '<br><b>Justificativa:</b> ' || avsobs
                                    ELSE ''
                                END
                                ||
                                CASE
                                    WHEN avdf.avdresposta <> '' THEN '<br><br><b>Resposta do Técnico:</b><br>' || avdf.avdresposta || '<br><b>Data: </b>' || to_char(avdf.avddtresposta::TIMESTAMP,'DD/MM/YYYY HH24:MI:SS')
                                    ELSE ''
                                END
                                || '</div>' AS avaliacao,";
			
			
			$campoAvaliacaoGroup = "CASE avdf.avdgeral
                                        WHEN '1' THEN 'Ruim'
                                        WHEN '2' THEN 'Regular'
                                        WHEN '3' THEN 'Bom'
                                        WHEN '4' THEN 'Ótimo'
                                        ELSE 'Não Avaliada'
                                    END AS avaliacaogrup,";
			
			$inneravaliacao = "INNER JOIN (SELECT a.dmdid, a.avdgeral, a.avddata, a.avsobs, a.avdresposta, a.avddtresposta
                                            FROM demandas.avaliacaodemanda a
                                            INNER JOIN ( SELECT dmdid, MAX(avdid) AS avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid
                                            WHERE a.avdgeral IN  ('3','2','1','4') 
                                            GROUP BY 1,2,3,4,5,6
                                        ) avdf ON avdf.dmdid = d.dmdid";
	}

	if( $avaliacao[0] && $avaliacao_campo_flag != '' ){
		//array_push($where, " avd.avdgeral " . (!$avaliacao_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $avaliacao ) . "') ");
		
		if(!$avaliacao_campo_excludente) $inn = "IN";
		else $inn = "NOT IN";

        $campoAvaliacao = "'<div align=left><b>Data: ' || to_char(avdf.avddata::TIMESTAMP,'DD/MM/YYYY HH24:MI:SS')
                            || '<br> Avaliação:</b> ' ||
                            CASE avdf.avdgeral
                                WHEN '1' THEN 'RUIM'
                                WHEN '2' THEN 'REGULAR'
                                WHEN '3' THEN 'BOM'
                                WHEN '4' THEN 'ÓTIMO'
                            END
                            ||
                            CASE
                                WHEN avsobs <> '' THEN '<br><b>Justificativa:</b> ' || avsobs
                                ELSE ''
                            END
                            ||
                            CASE
                                WHEN avdf.avdresposta <> '' THEN '<br><br><b>Resposta do Técnico:</b><br>' || avdf.avdresposta || '<br><b>Data: </b>' || to_char(avdf.avddtresposta::TIMESTAMP,'DD/MM/YYYY HH24:MI:SS')
                                ELSE ''
                            END
                            || '</div>' AS avaliacao,";
        
        $inneravaliacao = "INNER JOIN (SELECT a.dmdid, a.avdgeral, a.avddata, a.avsobs, a.avdresposta, a.avddtresposta
                                        FROM demandas.avaliacaodemanda a
                                        INNER JOIN ( SELECT dmdid, MAX(avdid) AS avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid
                                        WHERE a.avdgeral $inn " . " ('" . implode( "','", $avaliacao ) . "') " . " 
                                        GROUP BY 1,2,3,4,5,6
                                    ) avdf ON avdf.dmdid = d.dmdid";
	}	
	
	// período de abertura
	if( $dtinicio && $dtfim ){
		$dtinicio = formata_data_sql($dtinicio);
		$dtfim = formata_data_sql($dtfim);
		array_push($where, " d.dmddatainclusao BETWEEN '{$dtinicio} 00:00:00' AND '{$dtfim} 23:59:59' ");
	}
	
	// período da situação
	if( $dtinisit && $dtfimsit ){
		$dtinisit = formata_data_sql($dtinisit);
		$dtfimsit = formata_data_sql($dtfimsit);
		array_push($where, " a.htddata between '{$dtinisit} 00:00:00' and '{$dtfimsit} 23:59:59' ");
	}
	
	//Período de Registro de Horas:
	if( $dtinirh && $dtfimrh){
		$dtinirh = formata_data_sql($dtinirh);
		$dtfimrh = formata_data_sql($dtfimrh);
		array_push($where, " rh.data between '{$dtinirh} 00:00:00' and '{$dtfimrh} 23:59:59' ");
	}

	// período do atendimento finalizado
	if( $dtiniaf && $dtfimaf ){
		$dtiniaf = formata_data_sql($dtiniaf);
		$dtfimaf = formata_data_sql($dtfimaf);
		
		//$sqlperiodoaf = " dataatendfinalizado between '{$dtiniaf} 00:00:00' and '{$dtfimaf} 23:59:59' ";
		$sqlperiodoaf = " (SELECT MAX(htddata)
                           FROM workflow.historicodocumento a
			               WHERE a.aedid in (146, 191,224, 703) and docid=d.docid) between '{$dtiniaf} 00:00:00' and '{$dtfimaf} 23:59:59' ";
		array_push($where, $sqlperiodoaf);
	}
	
	// pesquisa em campos textos
	if( $pesquisacampo && $textopesquisa ){

		$textopesquisa = str_replace("'", "", $textopesquisa);
		/*
		if(in_array("assunto", $pesquisacampo)) array_push($where, " upper(d.dmdtitulo) ilike '%".strtoupper($textopesquisa)."%' ");
		if(in_array("descricao", $pesquisacampo)) array_push($where, " upper(d.dmddsc) ilike '%".strtoupper($textopesquisa)."%' ");
		if(in_array("servicoexecutado", $pesquisacampo)) array_push($where, " upper(hst.servico) ilike '%".strtoupper($textopesquisa)."%' ");
		if(in_array("solicitante", $pesquisacampo)) array_push($where, " ( upper(u.usunome) ilike '%".strtoupper($textopesquisa)."%' OR upper(d.dmdnomedemandante) ilike '%".strtoupper($textopesquisa)."%' ) ");
		if(in_array("tecnico", $pesquisacampo)) array_push($where, " upper(u2.usunome) ilike '%".strtoupper($textopesquisa)."%' ");
		if(in_array("gestor", $pesquisacampo)) array_push($where, " upper(ug.usunome) ilike '%".strtoupper($textopesquisa)."%' ");
		if(in_array("tempoadicional", $pesquisacampo)) array_push($where, " upper(d.dmdobstempoadicional) ilike '%".strtoupper($textopesquisa)."%' ");
		*/
		if(in_array("assunto", $pesquisacampo)) array_push($where, " upper(d.dmdtitulo) ilike '%".strtoupper($textopesquisa)."%' ");
		if(in_array("descricao", $pesquisacampo)) array_push($where, " upper(d.dmddsc) ilike '%".strtoupper($textopesquisa)."%' ");
		if(in_array("servicoexecutado", $pesquisacampo)) array_push($where, " upper(hst.servico) ilike '%".strtoupper($textopesquisa)."%' ");
		if(in_array("solicitante", $pesquisacampo)) array_push($where, " ( upper((SELECT u.usunome FROM seguranca.usuario u WHERE u.usucpf = d.usucpfdemandante)) ilike '%".strtoupper($textopesquisa)."%' OR upper(d.dmdnomedemandante) ilike '%".strtoupper($textopesquisa)."%' ) ");
		if(in_array("tecnico", $pesquisacampo)) array_push($where, " upper((SELECT u2.usunome FROM seguranca.usuario u2 WHERE u2.usucpf = d.usucpfexecutor)) ilike '%".strtoupper($textopesquisa)."%' ");
		if(in_array("gestor", $pesquisacampo)) array_push($where, " upper((SELECT ug.usunome FROM seguranca.usuario ug WHERE ug.usucpf = a2.usucpf)) ilike '%".strtoupper($textopesquisa)."%' ");
		if(in_array("tempoadicional", $pesquisacampo)) array_push($where, " upper(d.dmdobstempoadicional) ilike '%".strtoupper($textopesquisa)."%' ");

	}
	
	//PEGA SOMENTE AS DEMANDAS EM PAUSA
	if($chkpause){
		$chkpause = "INNER JOIN demandas.pausademanda pd ON pd.dmdid = d.dmdid";
	}
	
	//where para buscar somente as demandas em atraso
	if($demandaematraso){
		array_push($where, " d.dmddatafimprevatendimento < now() ");
		array_push($where, " d.dmdid not in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null ) ");
	}
	
	//exibe anos anteriores a 2014 e 2015
	if(!$anosanteriores){
		array_push($where, " to_char(d.dmddatainclusao,'YYYY') in ('2014','2015') ");
	}
	
	//ORDENAR CAMPOS
	if($_REQUEST['ordenarcampo']){
		$ORDENA = "";
		$ORDENA = implode(', ', $_REQUEST['ordenarcampo']);
		$ORDENA = str_replace("codigo","nudemanda ".$_REQUEST['tipoordenar'],$ORDENA);
		$ORDENA = str_replace("dataabertura","dmddatainclusao ".$_REQUEST['tipoordenar'],$ORDENA);
		$ORDENA = str_replace("datasituacao","datasituacaosql ".$_REQUEST['tipoordenar'],$ORDENA);
		$ORDENA = str_replace("dataconclusao","dataatendfinalizado ".$_REQUEST['tipoordenar'],$ORDENA);
		$ORDENA = str_replace("pontuacao","pontuacao ".$_REQUEST['tipoordenar'],$ORDENA);
		$ORDENA = str_replace("qtdservico","qtdservico ".$_REQUEST['tipoordenar'],$ORDENA);
	}else{
		$ORDENA = implode(', ', $_REQUEST['agrupador']);
		$ORDENA = str_replace("mesano","nudemanda, mesano",$ORDENA);
		$ORDENA = str_replace("prioridadeg","prioridade",$ORDENA);
		$ORDENA = str_replace("avaliacaog","avaliacao",$ORDENA);
	}	
	
	//SQL tunada atual
	$sql = "SELECT  DISTINCT
                LPAD(CAST(d.dmdid AS VARCHAR), GREATEST( LENGTH(CAST(d.dmdid AS VARCHAR)), 5), '0') AS nudemanda,
                od.ordid AS ordid,
                od.orddescricao AS origemdemanda,
                t.tipnome AS tipodemanda,
                smd.sidabrev || ' - ' || smd.siddescricao AS sistema,
                cel.celnome AS celula,
                CASE
                    WHEN doc.esdid IN (100,110) THEN (SELECT 
                                    (SELECT b.cmddsc
                                       FROM workflow.comentariodocumento b
                                       WHERE b.hstid = max(a.hstid)) AS servico
                        FROM workflow.historicodocumento a 
                        WHERE a.aedid in (146, 191,224, 703) and docid=d.docid)
                    WHEN doc.esdid IN (93,95,109,111,170) THEN (SELECT 
                                    (SELECT b.cmddsc
                                       FROM workflow.comentariodocumento b
                                       WHERE b.hstid = max(a.hstid)) AS servico
                        FROM workflow.historicodocumento a 
                        WHERE a.aedid in (146, 191,224, 703) and docid=d.docid)
                    ELSE ''
                END AS servicoexec,
                CASE
                    WHEN doc.esdid IN (100,110) THEN ''
                    WHEN doc.esdid IN (93,95,109,111,170) THEN (SELECT 
                                    to_char(MAX(a.htddata)::timestamp,'YYYY-MM-DD HH24:MI:00') AS datadoc
                        FROM workflow.historicodocumento a 
                        WHERE a.aedid in (146, 191,224, 703) and docid=d.docid)
                    ELSE to_char(now()::TIMESTAMP,'YYYY-MM-DD HH24:MI:00')
                END AS datadocfinalizada,
                CASE
                    WHEN doc.esdid IN (93,95,109,111,170) THEN (SELECT 
                                    to_char(MAX(htddata)::timestamp,'DD/MM/YYYY HH24:MI') AS dataconc
                        FROM workflow.historicodocumento a 
                        WHERE a.aedid in (146, 191,224, 703) and docid=d.docid)
                    ELSE ''
                END AS dataconclusao,
                to_char(a.htddata::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS datasituacao,  
                a.htddata AS datasituacaosql,
                (SELECT 
                                  MAX(htddata)
                        FROM workflow.historicodocumento a 
                        WHERE a.aedid in (146, 191,224, 703) and docid=d.docid) AS dataatendfinalizado,   
                '' AS observacao,             
                CASE
                    WHEN d.dmddatafimprevatendimento < now() AND doc.esdid IN (91,107,92,108) THEN 
                        '<FONT COLOR=RED>' || d.dmdtitulo || '</FONT>'
                    ELSE d.dmdtitulo
                END AS assunto,
                d.dmddsc AS descricao,
                to_char(d.dmddatainclusao::DATE,'DD/MM/YYYY') ||' '|| to_char(d.dmddatainclusao, 'HH24:MI') AS dataabertura,
                d.dmddatainclusao AS dmddatainclusao,
                to_char(d.dmddatainiprevatendimento::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS datainicio,
                to_char(d.dmddatafimprevatendimento::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS datafim,
                d.dmddatafimprevatendimento AS datafimordem,
                '' AS  prazoatendimento,
                '' AS  tempodecorrido,
                '' AS duracaoatendminutos,
                '' AS  tempopausa,
                to_char(d.dmddatafimprevatendimento::TIMESTAMP,'YYYY-MM-DD HH24:MI:00') AS dmddatafimprevatendimento,
                to_char(d.dmddatainiprevatendimento::TIMESTAMP,'YYYY-MM-DD HH24:MI:00') AS dmddatainiprevatendimento,
                CASE
                    WHEN d.dmdnomedemandante <> '' THEN d.dmdnomedemandante
                    ELSE (SELECT u.usunome FROM seguranca.usuario u WHERE u.usucpf = d.usucpfdemandante)
                END AS solicitante,
                CASE
                    WHEN d.usucpfexecutor <> '' THEN (SELECT u2.usunome FROM seguranca.usuario u2 WHERE u2.usucpf = d.usucpfexecutor)
                    ELSE 'Não informado'
                END AS tecnico,      
                (SELECT ug.usunome FROM seguranca.usuario ug WHERE ug.usucpf = a2.usucpf) AS gestor,  
                (SELECT u4.usunome FROM seguranca.usuario u4 WHERE u4.usucpf = d.usucpfclassificador) AS tecnicoclassif,
                (SELECT u5.usunome FROM seguranca.usuario u5 WHERE u5.usucpf = d.usucpfclassificadorsi) AS tecnicoclassifsi,
                d.dmddataclassificacao AS dataclassificacao,
                to_char(d.dmddataclassificacao::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS dataclassificacao2,
                d.dmddataclassificacaosi AS dataclassificacaosi,
                to_char(d.dmddataclassificacaosi::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS dataclassificacaosi2,     
                d.dmdnomedemandante AS demandantegeral,
                (SELECT '(' || u.usufoneddd || ') ' || u.usufonenum FROM seguranca.usuario u WHERE u.usucpf = d.usucpfdemandante) AS tel,
                upper(uni.unasigla) || ' - ' || uni.unadescricao AS setor,    
                loc.lcadescricao AS edificio,
                aa.anddescricao AS andar,
                d.dmdsalaatendimento AS sala,
                CASE d.dmdclassificacaosistema
                    WHEN '1' THEN 'Inicial'
                    WHEN '2' THEN 'Consultiva'
                    WHEN '3' THEN 'Investigativa'
                    WHEN '4' THEN 'Manutenção corretiva'
                    WHEN '5' THEN 'Manutenção evolutiva'
                    ELSE 'Não Classificada'
                END AS classifsistema,
                CASE d.dmdclassificacao
                    WHEN 'I' THEN 'Incidente'
                    WHEN 'P' THEN 'Resolução de problema'
                    WHEN 'M' THEN 'Requisição de mudança'
                    WHEN 'S' THEN 'Solicitação de Serviço'
                    ELSE 'Não Classificada'
                END AS classifdemanda,     

            $campoAvaliacaoGroup

            $campoAvaliacao
		 
            CASE
                WHEN ed.esddsc <> '' THEN ed.esddsc
                ELSE 'Em processamento'
            END AS situacao,      
            CASE
                WHEN p.pridsc <> '' THEN p.pridsc
                ELSE 'Não Informado'
            END AS prioridade,
            CASE EXTRACT(MONTH FROM d.dmddatainclusao)
                WHEN '1' THEN 'Janeiro/'
                WHEN '2' THEN 'Fevereiro/'
                WHEN '3' THEN 'Março/'
                WHEN '4' THEN 'Abril/'
                WHEN '5' THEN 'Maio/'
                WHEN '6' THEN 'Junho/'
                WHEN '7' THEN 'Julho/'
                WHEN '8' THEN 'Agosto/'
                WHEN '9' THEN 'Setembro/'
                WHEN '10' THEN 'Outubro/'
                WHEN '11' THEN 'Novembro/'
                WHEN '12' THEN 'Dezembro/'
            END || to_char(d.dmddatainclusao::DATE,'YYYY') AS mesano,
            d.dmdhorarioatendimento,
            CASE
                WHEN pt.tsppontuacao IS NOT NULL THEN pt.tsppontuacao
                ELSE '0' 
            END AS pontuacao,     
            '' AS tempoclassif,
            '' AS tempoclassifsi,
            '0' AS totalpontuacao,
            CASE
                WHEN d.dmdqtde > 0 THEN d.dmdqtde
                ELSE '1' 
            END AS qtdservico,
            '1' AS totaldemandas,
            CASE
                WHEN od.ordid in (18,19,20,21) THEN
            		lpad(pt.tsphora::varchar,3,'0') || ':' || lpad(pt.tspminuto::varchar,2,'0')
            	ELSE
            		lpad(pt.tsphora::varchar,2,'0') || ':' || lpad(pt.tspminuto::varchar,2,'0')
            END AS prazocatalogo,
            CASE
                WHEN od.ordid = 3 THEN '00:15'
                ELSE to_char(pt.tsptempoclassif, 'HH24:MI')  
            END AS tempoclassifcatalogo,
            to_char(d.dmdtempoadicional, 'HH24:MI') AS tempoadicional,
            d.dmdobstempoadicional AS justtempoadicional,
            d.dmdjustprioridade AS justprioridade,
            d.dmdjusturgente AS justurgente,
            '' AS nseriepatr,       
            '0' AS valordemanda,
            ed.esdid,
            '' AS qtdhoras,
            'Nº ' || d.scsid as nss,
            '' as histtramite,
            d.docid
    
		 FROM
		 demandas.demanda d
		 LEFT JOIN demandas.registroshorasdeatendimento rh ON d.dmdid = rh.dmdid
		 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
		 LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
		 LEFT JOIN demandas.prioridade p ON p.priid = d.priid					 
		 LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35) 
		 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid	

		 LEFT JOIN workflow.historicodocumento a ON a.hstid = doc.hstid 
		 LEFT JOIN workflow.comentariodocumento b on a.hstid = b.hstid
		 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
		 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid
		 LEFT JOIN  demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
		 LEFT JOIN  demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
		 LEFT JOIN  demandas.sistemadetalhe AS smd ON smd.sidid = d.sidid
		 LEFT JOIN  demandas.sistemacelula AS smc ON smc.sidid = d.sidid and smc.celid = d.celid
		 LEFT JOIN  demandas.celula AS cel ON cel.celid = smc.celid
		 $inneravaliacao
		 LEFT JOIN demandas.tiposervicoprioridade pt ON pt.tipid = d.tipid and pt.priid = d.priid and pt.tspstatus = 'A'
		 $chkpause
		 LEFT JOIN workflow.historicodocumento a2 ON a2.hstid = a.hstid AND a2.aedid in (224,186,165,278,279,368)
		 WHERE d.dmdstatus = 'A' " . ( $where[0] ? ' AND' . implode(' AND ', $where) : '' ) . "
		 ORDER BY  {$ORDENA}, datadocfinalizada";
// 		dbg($sql,1);
	return $sql;
	

}

function monta_agp(){
	$agrupador = $_REQUEST['agrupador'];
		$agp = array(
					"agrupador" => array(),
					"agrupadoColuna" => array(
												"solicitante",
												"origemdemanda",
												"tipodemanda",
												"assunto",
												"descricao",
												"dataabertura",
												"prazoatendimento",
												"prazocatalogo",
												"tempodecorrido",
												"duracaoatendminutos",
												"tempopausa",
												"dataconclusao",
												"datasituacao",
												"datainicio",
												"datafim",
												"tecnico",
												"gestor",
												"tecnicoclassif",
												"tecnicoclassifsi",
												"dataclassificacao2",
												"dataclassificacaosi2",
												"servicoexec",
												"avaliacao",
												"avaliacaogrup",
												"classifsistema",
												"situacao",
												"pontuacao",
												"tempoclassif",
												"tempoclassifcatalogo",
												"tempoclassifsi",
												"qtdservico",
												"totalpontuacao",
												"prioridade",
												"observacao",
												"tempoadicional",
												"justtempoadicional",
												"justprioridade",
												"justurgente",
												"nseriepatr",
												"valordemanda",
					   							"totaldemandas",
												"qtdhoras",
												"classifdemanda",
												"nss",
												"histtramite"
											  )	  
					);
	foreach ($agrupador as $val): 
	
		switch ($val) {		
		    case 'origemdemanda':
				array_push($agp['agrupador'], array(
													"campo" => "origemdemanda",
											 		"label" => "Origem")										
									   				);					
		    	continue;			
		        break;	
		        
		    case 'celula':
				array_push($agp['agrupador'], array(
												"campo" => "celula",
												"label" => "Célula")										
										   		);	
				continue;
				break;

		    case 'sistema':
				array_push($agp['agrupador'], array(
												"campo" => "sistema",
												"label" => "Sistema")										
										   		);	
				continue;
				break;
				
		    case 'setor':
				array_push($agp['agrupador'], array(
												"campo" => "setor",
												"label" => "Setor")										
										   		);	
				continue;
				break;	 
					    	
		    case 'solicitante':
				array_push($agp['agrupador'], array(
												"campo" => "solicitante",
												"label" => "Solicitante")										
										   		);	
				continue;
				break;	 
				
		    case 'tecnico':
				array_push($agp['agrupador'], array(
												"campo" => "tecnico",
												"label" => "Analista / Técnico")										
										   		);	
				continue;
				break;	 

		    case 'gestor':
				array_push($agp['agrupador'], array(
												"campo" => "gestor",
												"label" => "Gestor")										
										   		);	
				continue;
				break;	 
				
			
		    case 'tipodemanda':
				array_push($agp['agrupador'], array(
												"campo" => "tipodemanda",
												"label" => "Tipo da Demanda")										
										   		);	
				continue;
				break;	 
			
		    case 'prioridadeg':
				array_push($agp['agrupador'], array(
												"campo" => "prioridade",
												"label" => "Prioridade")										
										   		);	
				continue;
				break;	 

		    case 'situacao':
				array_push($agp['agrupador'], array(
												"campo" => "situacao",
												"label" => "Situação")										
										   		);	
				continue;
				break;	 

		    case 'avaliacaog':
				array_push($agp['agrupador'], array(
												"campo" => "avaliacaogrup",
												"label" => "Avaliação")										
										   		);	
				continue;
				break;	 

		    case 'classifsistema':
				array_push($agp['agrupador'], array(
												"campo" => "classifsistema",
												"label" => "Classificação Sistema")										
										   		);	
				continue;
				break;	 
				
		    case 'mesano':
				array_push($agp['agrupador'], array(
												"campo" => "mesano",
												"label" => "Mês / Ano")										
										   		);	
				continue;
				break;	 
			
			case 'classifdemanda':
				array_push($agp['agrupador'], array(
												"campo" => "classifdemanda",
												"label" => "Classificação da Demanda")										
										   		);	
				continue;
				break;	 
				
		}
	endforeach;

	
	array_push($agp['agrupador'], array(
									"campo" => "nudemanda",
							  		"label" => "Nº da Demanda")										
					   				);				
	return $agp;
}


function monta_coluna(){
	$colunaArr = (array) $_REQUEST['coluna'];
	
	
	$coluna = array();
	
	foreach($colunaArr as $campo):
		switch ($campo) {		
		    case 'solicitante':
				array_push($coluna,array(
													  "campo" => "solicitante",
											   		  "label" => "&nbsp;&nbsp;&nbsp;
											   		  			  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
											   		  			  Solicitante
											   		  			  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
											   		  			  &nbsp;&nbsp;&nbsp;"
													)										
									   			);					
		    	continue;			
		        break;	
		        
		    case 'origemdemanda':
				array_push($coluna, array(
													  "campo" => "origemdemanda",
											   		  "label" => "Origem"	
													)										
										   		);	
				continue;
				break;
				
		    case 'tipodemanda':
				array_push($coluna, array(
														"campo" => "tipodemanda",
												   		"label" => "Tipo da Demanda"
													)										
										   		);	
				continue;
				break;

		    case 'assunto':
				array_push($coluna, array(
														"campo" => "assunto",
												   		"label" => "Assunto"	
													)										
										   		);	
				continue;
				break;

		    case 'descricao':
				array_push($coluna, array(
														"campo" => "descricao",
												   		"label" => "Descrição"	
													)										
										   		);	
				continue;
				break;
				
		    case 'dataabertura':
				array_push($coluna, array(
														"campo" => "dataabertura",
												   		"label" => "Data<br>de Abertura"
													)										
										   		);	
				continue;
				break;
				
		    case 'datainicio':
				array_push($coluna, array(
														"campo" => "datainicio",
												   		"label" => "Previsão<br>de Início"	
													)										
										   		);	
				continue;
				break;
				
		    case 'datafim':
				array_push($coluna, array(
														"campo" => "datafim",
												   		"label" => "Previsão<br>de Término"	
													)										
										   		);	
				continue;
				break;
				
		    case 'prazoatendimento':
				array_push($coluna, array(
														"campo" => "prazoatendimento",
												   		"label" => "Prazo Previsto <br> Calculado (<font color=red>+ Pausa</font>)"	
													)										
										   		);	
				continue;
				break;

		    case 'prazocatalogo':
				array_push($coluna, array(
														"campo" => "prazocatalogo",
												   		"label" => "Prazo Previsto <br> Catálogo (<font color=blue>hh:mm</font>)"	
													)										
										   		);	
				continue;
				break;
				
		    case 'dataconclusao':
				array_push($coluna, array(
														"campo" => "dataconclusao",
												   		"label" => "Data<br>de Conclusão"	
													)										
										   		);	
				continue;
				break;
				
		    case 'datasituacao':
				array_push($coluna, array(
														"campo" => "datasituacao",
												   		"label" => "Data<br>da Situação"	
													)										
										   		);	
				continue;
				break;
				
		    case 'tempodecorrido':
				array_push($coluna, array(
													   "campo" => "tempodecorrido",
											   		   "label" => "Duração<br>do Atendimento"	
													)										
										   		);	
				continue;
				break;																

		    case 'duracaoatendminutos':
				array_push($coluna, array(
													   "campo" => "duracaoatendminutos",
											   		   "label" => "Duração do Atendimento<br>em Minutos",
													   "type"  => "numeric"	
													)										
										   		);	
				continue;
				break;																
				
		    case 'tempopausa':
				array_push($coluna, array(
													   "campo" => "tempopausa",
											   		   "label" => "Tempo da Pausa"	
													)										
										   		);	
				continue;
				break;																
				
		    case 'tecnico':
				array_push($coluna, array(
														"campo" => "tecnico",
												   		"label" => "Analista / Técnico"	
													)										
										   		);	
				continue;
				break;																

		    case 'gestor':
				array_push($coluna, array(
														"campo" => "gestor",
												   		"label" => "Gestor"	
													)										
										   		);	
				continue;
				break;			

		    case 'tecnicoclassif':
				array_push($coluna, array(
														"campo" => "tecnicoclassif",
												   		"label" => "Técnico(a) Classificador(a)"	
													)										
										   		);	
				continue;
				break;			

		    case 'dataclassif':
				array_push($coluna, array(
														"campo" => "dataclassificacao2",
												   		"label" => "Data da Classificação"	
													)										
										   		);	
				continue;
				break;			

		    case 'tecnicoclassifsi':
				array_push($coluna, array(
														"campo" => "tecnicoclassifsi",
												   		"label" => "Técnico(a) Classificador(a) SI"	
													)										
										   		);	
				continue;
				break;			

		    case 'dataclassifsi':
				array_push($coluna, array(
														"campo" => "dataclassificacaosi2",
												   		"label" => "Data da Classificação SI"	
													)										
										   		);	
				continue;
				break;			
				
		    case 'servicoexec':
				array_push($coluna, array(
														"campo" => "servicoexec",
												   		"label" => "Serviço Executado"	
													)										
										   		);	
				continue;
				break;																
				
		    case 'avaliacao':
				array_push($coluna, array(
														"campo" => "avaliacao",
												   		"label" => "Avaliação"	
													)										
										   		);	
				continue;
				break;																
				
		    case 'situacao':
				array_push($coluna, array(
														  "campo" => "situacao",
												   		  "label" => "Situação"	
													)										
										   		);	
				continue;
				break;

		    case 'pontuacao':
				array_push($coluna, array(
														  "campo" => "pontuacao",
												   		  "label" => "Pontuação",
														  "type"  => "numeric"
													)										
										   		);	
				continue;
				break;
				
		    case 'totalpontuacao':
				array_push($coluna, array(
														  "campo" => "totalpontuacao",
												   		  "label" => "Total de Pontuação",
														  "type"  => "numeric"
													)										
										   		);	
				continue;
				break;
				
		    case 'tempoclassif':
				array_push($coluna, array(
														  "campo" => "tempoclassif",
												   		  "label" => "Tempo da Classificação"	
													)										
										   		);	
				continue;
				break;

		    case 'tempoclassifcatalogo':
				array_push($coluna, array(
														  "campo" => "tempoclassifcatalogo",
												   		  "label" => "Tempo da Classificação Catálogo SI"	
													)										
										   		);	
				continue;
				break;
				
		    case 'tempoclassifsi':
				array_push($coluna, array(
														  "campo" => "tempoclassifsi",
												   		  "label" => "Tempo da Classificação SI"	
													)										
										   		);	
				continue;
				break;
				
		    case 'tempoadicional':
				array_push($coluna, array(
														  "campo" => "tempoadicional",
												   		  "label" => "Tempo Adicional Atendimento"	
													)										
										   		);	
				continue;
				break;

		    case 'justtempoadicional':
				array_push($coluna, array(
														  "campo" => "justtempoadicional",
												   		  "label" => "Justificativa do Tempo Adicional"	
													)										
										   		);	
				continue;
				break;

		    case 'justprioridade':
				array_push($coluna, array(
														  "campo" => "justprioridade",
												   		  "label" => "Justificativa da Prioridade"	
													)										
										   		);	
				continue;
				break;
				
		    case 'justurgente':
				array_push($coluna, array(
														  "campo" => "justurgente",
												   		  "label" => "Justificativa da Demanda Urgente"	
													)										
										   		);	
				continue;
				break;
				
		    case 'nseriepatr':
				array_push($coluna, array(
														  "campo" => "nseriepatr",
												   		  "label" => "Nº Série / Nº Patrimônio"	
													)										
										   		);	
				continue;
				break;
				
		    case 'valordemanda':
				array_push($coluna, array(
														  "campo" => "valordemanda",
												   		  "label" => "Valor da Demanda (R$)"	
													)										
										   		);	
				continue;
				break;
				
		    case 'qtdservico':
				array_push($coluna, array(
														  "campo" => "qtdservico",
												   		  "label" => "Quantidade de Serviço",
														  "type"  => "numeric"	
													)										
										   		);	
				continue;
				break;

		    case 'totaldemandas':
				array_push($coluna, array(
														  "campo" => "totaldemandas",
												   		  "label" => "Total de Demandas",
														  "type"  => "numeric"	
													)										
										   		);	
				continue;
				break;

		    case 'qtdhoras':
				array_push($coluna, array(
														  "campo" => "qtdhoras",
												   		  "label" => "Qtd. Horas Atendimento"
													)										
										   		);	
				continue;
				break;				
				
				
		    case 'prioridade':
				array_push($coluna, array(
														  "campo" => "prioridade",
												   		  "label" => "Prioridade"
													)										
										   		);	
				continue;
				break;
				
		    case 'observacao':
				array_push($coluna, array(
														  "campo" => "observacao",
												   		  "label" => "Observação"	
													)										
										   		);	
				continue;
				break;				

		    case 'classifdemanda':
				array_push($coluna, array(
														  "campo" => "classifdemanda",
												   		  "label" => "Classificação da Demanda"	
													)										
										   		);	
				continue;
				break;
					
			 case 'nss':
				array_push($coluna, array(
														  "campo" => "nss",
												   		  "label" => "Solicitação de Serviço (SS)"	
													)										
										   		);	
				continue;
				break;		

			case 'histtramite':
				array_push($coluna, array(
														  "campo" => "histtramite",
												   		  "label" => "Histórico de Tramites",
														  "type"  => "text"		
													)										
										   		);	
				continue;
				break;	
				
		 }
	endforeach;		

	return $coluna;			  	
}
?>