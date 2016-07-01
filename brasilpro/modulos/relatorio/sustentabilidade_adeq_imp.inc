<html>
	<head>
		<title>Relatório Sustentabilidade e Adequação</title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
		<style type="text/css">
			body{ margin: 2; padding: 2; }
		</style>
		<style>
			 @media print {
			 .notprint { display: none }
			 }
		</style>
		<style>
		<!--
		table.ex
		{
		color:#000000;
		background-color:#f1f1f1;
		font-size: 100%;
		padding:0px;
		border-top: 1px solid black;
		border-left: 1px solid black;
		border-bottom: 0px solid black;
		border-right: 0px solid black;
		}
		
		table.ex th, table.ex td 
		{
		padding-bottom:2px;
		border-top: 0px solid black;
		border-left: 0px solid black;
		border-bottom: 1px solid black;
		border-right: 1px solid black;
		}
		-->
		</style>
		<script type="text/javascript">
			top.window.focus();
		</script>
	</head>

<body>

		
<form action="" method="post" name="formulario">
	<input type="hidden" name="buscar" value="1">
	
	<div class="notprint">
	<table width="100%">
		<tr>
			<td align="right">
				<input type="button" value="Imprimir" style="cursor: pointer" onclick="self.print();">
			</td>
		</tr>
	</table>
	</div>

	<table width="100%" cellspacing="1" cellpadding="1" bgcolor="#f5f5f5" border="0" align="center" class="listagem">		
		<tr>
			<td width="100%" align="center" colspan="2" style="background-color: #dcdcdc;">
				<br>
				<label class="TituloTela" style="color:#000000;">Relatório de Sustentabilidade e Adequação</label>
				<br><br>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top" width="30%">
				<b>Estado:</b>
			</td>
			<td>
				<?
				$estuf = $_REQUEST["estuf"];
				echo $db->pegaUm("select upper(estdescricao)	
								from territorios.estado 
								where estuf = '".$estuf."'
								");
				?>
			</td>
    	</tr>
		<tr>
			<td class="SubTituloDireita" valign="top" >
				<b>Ano:</b>
			</td>
			<td>
				<?=implode(", ",$_REQUEST['chkano'])?>
			</td>
    	</tr>
		<tr>
			<td class="subtitulodireita" valign="top" ><b>Secretário(a) Educação:</b></td>
			<td>
				<?//=strtoupper($_REQUEST['secretario_educacao'])?>
				<?
			 		$secretario = array();

					$secretario = $db->carregar("select ent.entnome, fun.fundsc, ent1.entnome as nome,  fun1.fundsc
												from entidade.entidade ent
												INNER JOIN entidade.funcaoentidade fe ON fe.entid = ent.entid
												inner join entidade.funentassoc ea on fe.fueid = ea.fueid
												inner join entidade.funcao fun on fun.funid = fe.funid
												inner join entidade.endereco ende on ent.entid = ende.entid
												inner join entidade.entidade ent1 on ent1.entid = ea.entid
												INNER JOIN entidade.funcaoentidade fe1 ON fe1.entid = ent1.entid
												inner join entidade.funcao fun1 on fun1.funid = fe1.funid
												where fe.funid = 6
												and ende.estuf = '".$estuf."' 
											");
					/*
					if($secretario){
						foreach($secretario as $secretario){
							echo strtoupper($secretario['nome']) . '<br>';
						}
					}
					*/
					echo strtoupper($secretario[0]['nome']);
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita" valign="top" ><b>Secretário(a) Ciência e Tecnologia:</b></td>
			<td><?=strtoupper($secretario[1]['nome'])?></td>
		</tr>
		<tr>
			<td class="subtitulodireita" colspan="2" >&nbsp;</td>
		</tr>
	</table>	
	
	
	
<?if ( $_REQUEST['buscar'] ){
//echo implode("','",$_REQUEST['chkano']);
	


	//matriculas_censo
	$matriculas_censo = $db->pegaUm("select  sum(coalesce(cmtvlr,0)) as total
							from cte.censomatricula 
							where estuf = '".$estuf."'
							and cmtano = '2006'
							and cmttipo = 'E'
							");


	$PorEscola = array();
	$Global = array();
	$Total1 = array();
	for($i=1; $i<11; $i++){
	
		//global
		$Global[$i] = $db->pegaUm("select  sum(coalesce(spt.sptunt,0)) as total
								from cte.dimensao d 
								inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' 
								inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' 
								inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A' 
								inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
								inner join cte.acaoindicador ai on ai.ptoid = p.ptoid 
								inner join cte.subacaoindicador sai on sai.aciid = ai.aciid and sai.foaid = 1 and sai.sbaporescola = 'f'
								inner join cte.subacaoparecertecnico spt on spt.sbaid = sai.sbaid and spt.sptano in ('".implode("','",$_REQUEST['chkano'])."') 
								where d.dimstatus = 'A' 
								and d.dimcod = 1
								and ad.ardcod = 3
								and i.indcod = ".$i."
								");
					
		//por escola
		$PorEscola[$i] = $db->pegaUm("select  sum(coalesce(q.qfaqtd,0)) as total
								from cte.dimensao d 
								inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' 
								inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' 
								inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A' 
								inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
								inner join cte.acaoindicador ai on ai.ptoid = p.ptoid 
								inner join cte.subacaoindicador sai on sai.aciid = ai.aciid and sai.foaid = 1 and (sai.sbaporescola = 't' or sai.sbaporescola is null)
								inner join cte.qtdfisicoano q on q.sbaid = sai.sbaid and q.qfaano in ('".implode("','",$_REQUEST['chkano'])."')
								where d.dimstatus = 'A' 
								and d.dimcod = 1
								and ad.ardcod = 3
								and i.indcod = ".$i."
								");
	
		//pega o total global + por escola
		$Total1[$i] = $Global[$i] + $PorEscola[$i];
		$Total_vagas_EPT = $Total_vagas_EPT + $Total1[$i];
		
		if($i!=8 && $i!=9 && $i!=10){
			$Total_vagas_EMI = $Total_vagas_EMI + $Total1[$i];
		}  
		/*
		if($i!=8){
			$Total_suportar_ampliacao = $Total_suportar_ampliacao + $Total1[$i];
		} 
		*/ 
		
	}

	
	//2.1
	//global
	$Prof_serem_contr_global = $db->pegaUm("select  sum(coalesce(spt.sptunt,0)) as total
							from cte.dimensao d 
							inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' 
							inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' 
							inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A' 
							inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
							inner join cte.acaoindicador ai on ai.ptoid = p.ptoid 
							inner join cte.subacaoindicador sai on sai.aciid = ai.aciid and sai.foaid = 1 and sai.sbaporescola = 'f'
							inner join cte.subacaoparecertecnico spt on spt.sbaid = sai.sbaid and spt.sptano in ('".implode("','",$_REQUEST['chkano'])."') 
							where d.dimstatus = 'A' 
							and d.dimcod = 1
							and ad.ardcod = 2
							and i.indcod = 4
							");
				
	//por escola
	$Prof_serem_contr_PorEscola = $db->pegaUm("select  sum(coalesce(q.qfaqtd,0)) as total
							from cte.dimensao d 
							inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' 
							inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' 
							inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A' 
							inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
							inner join cte.acaoindicador ai on ai.ptoid = p.ptoid 
							inner join cte.subacaoindicador sai on sai.aciid = ai.aciid and sai.foaid = 1 and (sai.sbaporescola = 't' or sai.sbaporescola is null)
							inner join cte.qtdfisicoano q on q.sbaid = sai.sbaid and q.qfaano in ('".implode("','",$_REQUEST['chkano'])."')
							where d.dimstatus = 'A' 
							and d.dimcod = 1
							and ad.ardcod = 2
							and i.indcod = 4
							");
	
	$Prof_serem_contr_Total =  $Prof_serem_contr_global + $Prof_serem_contr_PorEscola;
	
	
	//2.2
	//global
	$Prof_nucleo_basico_global = $db->pegaUm("select  sum(coalesce(spt.sptunt,0)) as total
							from cte.dimensao d 
							inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' 
							inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' 
							inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A' 
							inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
							inner join cte.acaoindicador ai on ai.ptoid = p.ptoid 
							inner join cte.subacaoindicador sai on sai.aciid = ai.aciid and sai.foaid = 1 and sai.undid = 582 and sai.sbaporescola = 'f'
							inner join cte.subacaoparecertecnico spt on spt.sbaid = sai.sbaid and spt.sptano in ('".implode("','",$_REQUEST['chkano'])."') 
							where d.dimstatus = 'A' 
							and d.dimcod = 1
							and ad.ardcod = 2
							and i.indcod = 4
							");
				
	//por escola
	$Prof_nucleo_basico_PorEscola = $db->pegaUm("select  sum(coalesce(q.qfaqtd,0)) as total
							from cte.dimensao d 
							inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' 
							inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' 
							inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A' 
							inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
							inner join cte.acaoindicador ai on ai.ptoid = p.ptoid 
							inner join cte.subacaoindicador sai on sai.aciid = ai.aciid and sai.foaid = 1 and sai.undid = 582 and (sai.sbaporescola = 't' or sai.sbaporescola is null)
							inner join cte.qtdfisicoano q on q.sbaid = sai.sbaid and q.qfaano in ('".implode("','",$_REQUEST['chkano'])."')
							where d.dimstatus = 'A' 
							and d.dimcod = 1
							and ad.ardcod = 2
							and i.indcod = 4
							");
	
	$Prof_nucleo_basico_Total =  $Prof_nucleo_basico_global + $Prof_nucleo_basico_PorEscola;
	


	
	//2.3
	//global
	$Prof_area_tec_global = $db->pegaUm("select  sum(coalesce(spt.sptunt,0)) as total
							from cte.dimensao d 
							inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' 
							inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' 
							inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A' 
							inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
							inner join cte.acaoindicador ai on ai.ptoid = p.ptoid 
							inner join cte.subacaoindicador sai on sai.aciid = ai.aciid and sai.foaid = 1 and sai.undid = 583 and sai.sbaporescola = 'f'
							inner join cte.subacaoparecertecnico spt on spt.sbaid = sai.sbaid and spt.sptano in ('".implode("','",$_REQUEST['chkano'])."') 
							where d.dimstatus = 'A' 
							and d.dimcod = 1
							and ad.ardcod = 2
							and i.indcod = 4
							");
				
	//por escola
	$Prof_area_tec_PorEscola = $db->pegaUm("select  sum(coalesce(q.qfaqtd,0)) as total
							from cte.dimensao d 
							inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' 
							inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' 
							inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A' 
							inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
							inner join cte.acaoindicador ai on ai.ptoid = p.ptoid 
							inner join cte.subacaoindicador sai on sai.aciid = ai.aciid and sai.foaid = 1 and sai.undid = 583 and (sai.sbaporescola = 't' or sai.sbaporescola is null)
							inner join cte.qtdfisicoano q on q.sbaid = sai.sbaid and q.qfaano in ('".implode("','",$_REQUEST['chkano'])."')
							where d.dimstatus = 'A' 
							and d.dimcod = 1
							and ad.ardcod = 2
							and i.indcod = 4
							");
	
	$Prof_area_tec_Total =  $Prof_area_tec_global + $Prof_area_tec_PorEscola;
	
	
	
	
	$PorEscola2 = array();
	$Global2 = array();
	$Total2 = array();
	for($i=1; $i<7; $i++){
	
		//global
		$Global2[$i] = $db->pegaUm("select  sum(coalesce(spt.sptunt,0)) as total
								from cte.dimensao d 
								inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' 
								inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' 
								inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A' 
								inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
								inner join cte.acaoindicador ai on ai.ptoid = p.ptoid 
								inner join cte.subacaoindicador sai on sai.aciid = ai.aciid and sai.foaid = 1 and sai.sbaporescola = 'f'
								inner join cte.subacaoparecertecnico spt on spt.sbaid = sai.sbaid and spt.sptano in ('".implode("','",$_REQUEST['chkano'])."') 
								where d.dimstatus = 'A' 
								and d.dimcod = 2
								and ad.ardcod = 1
								and i.indcod = ".$i."
								");
					
		//por escola
		$PorEscola2[$i] = $db->pegaUm("select  sum(coalesce(q.qfaqtd,0)) as total
								from cte.dimensao d 
								inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' 
								inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' 
								inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A' 
								inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
								inner join cte.acaoindicador ai on ai.ptoid = p.ptoid 
								inner join cte.subacaoindicador sai on sai.aciid = ai.aciid and sai.foaid = 1 and (sai.sbaporescola = 't' or sai.sbaporescola is null)
								inner join cte.qtdfisicoano q on q.sbaid = sai.sbaid and q.qfaano in ('".implode("','",$_REQUEST['chkano'])."')
								where d.dimstatus = 'A' 
								and d.dimcod = 2
								and ad.ardcod = 1
								and i.indcod = ".$i."
								");
	
		//pega o total global + por escola
		$Total2[$i] = $Global2[$i] + $PorEscola2[$i];
		
	}
	
	$prof_cap_inicial = $Total2[1] + $Total2[2] + $Total2[3] + $Total2[4] + $Total2[5];
	$equip_pedagog_cap_inicial = $Total2[6]; 
	
	

	
	$PorEscola22 = array();
	$Global22 = array();
	$Total22 = array();
	for($i=1; $i<9; $i++){
	
		//global
		$Global22[$i] = $db->pegaUm("select  sum(coalesce(spt.sptunt,0)) as total
								from cte.dimensao d 
								inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' 
								inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' 
								inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A' 
								inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
								inner join cte.acaoindicador ai on ai.ptoid = p.ptoid 
								inner join cte.subacaoindicador sai on sai.aciid = ai.aciid and sai.foaid = 1 and sai.sbaporescola = 'f'
								inner join cte.subacaoparecertecnico spt on spt.sbaid = sai.sbaid and spt.sptano in ('".implode("','",$_REQUEST['chkano'])."') 
								where d.dimstatus = 'A' 
								and d.dimcod = 2
								and ad.ardcod = 2
								and i.indcod = ".$i."
								");
					
		//por escola
		$PorEscola22[$i] = $db->pegaUm("select  sum(coalesce(q.qfaqtd,0)) as total
								from cte.dimensao d 
								inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' 
								inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' 
								inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A' 
								inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
								inner join cte.acaoindicador ai on ai.ptoid = p.ptoid 
								inner join cte.subacaoindicador sai on sai.aciid = ai.aciid and sai.foaid = 1 and (sai.sbaporescola = 't' or sai.sbaporescola is null)
								inner join cte.qtdfisicoano q on q.sbaid = sai.sbaid and q.qfaano in ('".implode("','",$_REQUEST['chkano'])."')
								where d.dimstatus = 'A' 
								and d.dimcod = 2
								and ad.ardcod = 2
								and i.indcod = ".$i."
								");
	
		//pega o total global + por escola
		$Total22[$i] = $Global22[$i] + $PorEscola22[$i];
		
	}
	
	$prof_cap_continuada = $Total22[1] + $Total22[2] + $Total22[3] + $Total22[4] + $Total22[5];
	$equip_pedagog_cap_continuada = $Total22[6] + $Total22[7];
	$tec_laboratorio_cap = $Total22[8];
	

	
	//4.1 numero total de escolas
	$total_escolas_atendidas = $db->pegaUm("select distinct count(coalesce(ie.entid,0)) as total
									from cte.instrumentounidadeescola ie
									inner join cte.instrumentounidade i on i.inuid=ie.inuid and i.itrid in (3) and i.estuf = '".$estuf."' 
									");	
	
	
	//calcula percentual ppps***************************************************
		//total de itens menos(itens igual a 6,7,10) com ept
		$total_itens_ppps = $db->pegaUm("select count(coalesce(ippid,0)) as total 
										from cte.itensppp 
										where ippconteudo = '1' and ippid not in(28,29,30,32,33,34,42) 
										");	
		
		//pega o id das escolas com ept 
		$lista_id_escolas_com_ept = array();
		/*
		$lista_id_escolas_com_ept = $db->carregar("select distinct ie.entid as id
										from cte.instrumentounidadeescola ie
										inner join entidade.entidadedetalhe ed on ed.entid=ie.entid and ed.entdoferta_ept = 't'
										inner join cte.instrumentounidade iu on iu.inuid=ie.inuid and iu.estuf = '".$estuf."' 
										");
		*/
		$lista_id_escolas_com_ept = $db->carregar("select distinct ie.entid as id
										from cte.instrumentounidadeescola ie
										inner join entidade.entidadedetalhe ed on ed.entid=ie.entid and ed.entdoferta_ept = 't'
										inner join cte.instrumentounidade iu on iu.inuid=ie.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
										");
		
		if($lista_id_escolas_com_ept){
			foreach($lista_id_escolas_com_ept as $lista_id_escolas_com_ept2){
				//pega total de itens de uma escola
				$total_itens_ppps_com_ept = $db->pegaUm("select count(coalesce(cp.ippid,0)) as total from cte.conteudoppp cp
												inner join entidade.endereco e on e.entid = cp.entid  and cp.entid = ".$lista_id_escolas_com_ept2['id']." 
												");	

				if($total_itens_ppps_com_ept >= $total_itens_ppps){
				 	$qtd_ppps_completos_com_ept = $qtd_ppps_completos_com_ept + 1; 
				}
				
			}
			
			$porcentagem_ppps_com_ept = ($qtd_ppps_completos_com_ept * 100) / sizeof($lista_id_escolas_com_ept);
			
		}
		else $porcentagem_ppps_com_ept = 0;
		

				
		//total de itens menos(itens igual a 6,7,10) sem ept(campos nao bloqueados ippbloqdifept = 't' or is null) 
		$total_itens_ppps2 = $db->pegaUm("select count(coalesce(ippid,0)) as total 
										from cte.itensppp 
										where ippconteudo = '1' and ippid not in(28,29,30,32,33,34,42) and (ippbloqdifept = 'f' or ippbloqdifept is null) 
										");	
		
		//pega o id das escolas sem ept 
		$lista_id_escolas_sem_ept = array();
		$lista_id_escolas_sem_ept = $db->carregar("select distinct ie.entid as id
										from cte.instrumentounidadeescola ie
										inner join entidade.entidadedetalhe ed on ed.entid=ie.entid and (ed.entdoferta_ept = 'f' or ed.entdoferta_ept is null)
										inner join cte.instrumentounidade iu on iu.inuid=ie.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
										");
		if($lista_id_escolas_sem_ept){
			foreach($lista_id_escolas_sem_ept as $lista_id_escolas_sem_ept2){
				//pega total de itens de uma escola
				$total_itens_ppps_sem_ept = $db->pegaUm("select count(coalesce(cp.ippid,0)) as total from cte.conteudoppp cp
												inner join entidade.endereco e on e.entid = cp.entid and cp.entid = ".$lista_id_escolas_sem_ept2['id']." 
												");	
				
				 if($total_itens_ppps_sem_ept >= $total_itens_ppps2){
				 	$qtd_ppps_completos_sem_ept = $qtd_ppps_completos_sem_ept + 1; 
				 }
				
			}
			
			$porcentagem_ppps_sem_ept = ($qtd_ppps_completos_sem_ept * 100) / sizeof($lista_id_escolas_sem_ept);
			
		}
		else $porcentagem_ppps_sem_ept = 0;

	//fim calcula percentual ppps***************************************************
		
		
	$total_escolas_com_ept = $db->pegaUm("select distinct count(coalesce(ie.entid,0)) as total
										from cte.instrumentounidadeescola ie
										inner join entidade.entidadedetalhe ed on ed.entid=ie.entid and ed.entdoferta_ept = 't'
										inner join cte.instrumentounidade iu on iu.inuid=ie.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
										");	

	$total_escolas_sem_ept = $db->pegaUm("select distinct count(coalesce(ie.entid,0)) as total
										from cte.instrumentounidadeescola ie
										inner join entidade.entidadedetalhe ed on ed.entid=ie.entid and (ed.entdoferta_ept = 'f' or ed.entdoferta_ept is null)
										inner join cte.instrumentounidade iu on iu.inuid=ie.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
										");	
	
	
	//4.3
	$lista_cursos_mais_solicitados = array();
	$lista_cursos_mais_solicitados = $db->carregar("SELECT cr.crstitulo as nome, count(cr.crstitulo) as qtd
										        FROM cte.conteudopppcursotecnico cc
        										INNER JOIN cte.cursotecnico cr ON cc.crsid = cr.crsid
												INNER JOIN cte.conteudoppp cp ON cc.cppid = cp.cppid
												INNER JOIN cte.instrumentounidadeescola ie ON cp.entid = ie.entid
												inner join cte.instrumentounidade iu on iu.inuid=ie.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
												group by cr.crstitulo
												order by 2 desc 
												limit 5	
												");	
												/*
												--sustentabilidade
												SELECT cr.crstitulo, count(cr.crsid)
												FROM cte.conteudopppcursotecnico cc
												inner JOIN cte.cursotecnico cr ON cc.crsid = cr.crsid
												inner JOIN cte.conteudoppp cp ON cc.cppid = cp.cppid AND cp.cppid IS NOT NULL 
												inner JOIN cte.instrumentounidadeescola ie ON cp.entid = ie.entid 
												right join cte.instrumentounidade iu on iu.inuid=ie.inuid and iu.itrid=3 and iu.estuf = 'DF' 
												inner JOIN entidade.entidade e ON e.entid = ie.entid 
												where e.tpcid = 1 
												AND e.funid = 3 
												group by cr.crstitulo
												order by 2 desc 	
												*/	
	
	//4.4
	$Escolas_finan_PROEP = $db->pegaUm("select  count(coalesce(q.entid,0)) as total
										from cte.dimensao d 
										inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' 
										inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' 
										inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A' 
										inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
										inner join cte.acaoindicador ai on ai.ptoid = p.ptoid 
										inner join cte.subacaoindicador sai on sai.aciid = ai.aciid and sai.foaid = 1 and (sai.sbaporescola = 't' or sai.sbaporescola is null)
										inner join cte.qtdfisicoano q on q.sbaid = sai.sbaid and q.qfaano in ('".implode("','",$_REQUEST['chkano'])."')
										where d.dimstatus = 'A' 
										and d.dimcod = 1
										and ad.ardcod = 3
										and i.indcod = 2
										");


	//4.4.1
	$escolas_proep_parecer = array();
	$escolas_proep_parecer = $db->carregar("select ep.entid, ep.esppar as parecer, e.entnome as nome,
										        mu.muncod,
										        mu.mundescricao
										    from cte.escolaproep ep
										        inner join entidade.entidade e on
										            e.entid = ep.entid
										        inner join entidade.endereco ende on
										            ende.entid = e.entid
										        inner join territorios.municipio mu on
										            mu.muncod = ende.muncod
										    where
											ep.espano in ('".implode("','",$_REQUEST['chkano'])."') 
										        and e.entproep = true
											and ende.estuf = '".$estuf."' 	
												");	
	
	
	//5.1
	$PorEscola5 = array();
	$Global5 = array();
	$Total5 = array();
	for($i=1; $i<2; $i++){
	
		//global
		$Global5[$i] = $db->pegaUm("select  sum(coalesce(spt.sptunt,0)) as total
								from cte.dimensao d 
								inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' 
								inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' 
								inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A' 
								inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
								inner join cte.acaoindicador ai on ai.ptoid = p.ptoid 
								inner join cte.subacaoindicador sai on sai.aciid = ai.aciid and sai.foaid = 1 and sai.sbaporescola = 'f'
								inner join cte.subacaoparecertecnico spt on spt.sbaid = sai.sbaid and spt.sptano in ('".implode("','",$_REQUEST['chkano'])."') 
								where d.dimstatus = 'A' 
								and d.dimcod = 4
								and ad.ardcod = 2
								and i.indcod = ".$i."
								");
					
		//por escola
		$PorEscola5[$i] = $db->pegaUm("select  sum(coalesce(q.qfaqtd,0)) as total
								from cte.dimensao d 
								inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' 
								inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' 
								inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A' 
								inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
								inner join cte.acaoindicador ai on ai.ptoid = p.ptoid 
								inner join cte.subacaoindicador sai on sai.aciid = ai.aciid and sai.foaid = 1 and (sai.sbaporescola = 't' or sai.sbaporescola is null)
								inner join cte.qtdfisicoano q on q.sbaid = sai.sbaid and q.qfaano in ('".implode("','",$_REQUEST['chkano'])."')
								where d.dimstatus = 'A' 
								and d.dimcod = 4
								and ad.ardcod = 2
								and i.indcod = ".$i."
								");
	
		//pega o total global + por escola
		$Total5[$i] = $Global5[$i] + $PorEscola5[$i];
		
	}
	
	//5.2
	$n_escolas_ead = $db->pegaUm("select  count(coalesce(q.entid,0)) as total
								from cte.dimensao d 
								inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' 
								inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' 
								inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A' 
								inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
								inner join cte.acaoindicador ai on ai.ptoid = p.ptoid 
								inner join cte.subacaoindicador sai on sai.aciid = ai.aciid and sai.foaid = 1 and (sai.sbaporescola = 't' or sai.sbaporescola is null)
								inner join cte.qtdfisicoano q on q.sbaid = sai.sbaid and q.qfaano in ('".implode("','",$_REQUEST['chkano'])."')
								where d.dimstatus = 'A' 
								and d.dimcod = 4
								and ad.ardcod = 2
								and i.indcod = 4
								");
	
	//5.3
	$n_polos_ead = $db->pegaUm("select count(coalesce(cs.cosid,0)) as total
								from cte.dimensao d 
								inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' 
								inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' 
								inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A' 
								inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
								inner join cte.acaoindicador ai on ai.ptoid = p.ptoid 
								inner join cte.subacaoindicador sai on sai.aciid = ai.aciid and sai.foaid = 1 
								inner join cte.composicaosubacao cs on cs.sbaid = sai.sbaid and cs.cosano in ('".implode("','",$_REQUEST['chkano'])."')
								where d.dimstatus = 'A' 
								and d.dimcod = 1
								and ad.ardcod = 3
								and i.indcod = 8
								");
	

	
	/*
	$lista_cursos_ead = array();
	$lista_cursos_ead = $db->carregar("select  um.unddsc as nome
													from cte.dimensao d 
													inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' 
													inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' 
													inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A' 
													inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.estuf = '".$estuf."' 
													inner join cte.acaoindicador ai on ai.ptoid = p.ptoid 
													inner join cte.subacaoindicador sai on sai.aciid = ai.aciid --and sai.foaid = 1
													inner join cte.unidademedida um on um.undid = sai.undid and um.undtipo = 'E' and substring(unddsc, 1, 22) = 'Alunos de Ead no Curso'
													where d.dimstatus = 'A' 
													and d.dimcod = 1
													and ad.ardcod = 3
													and i.indcod = 8	
													");	
	
	*/
	//6. laboratorios
	$PorEscola6 = array();
	$Global6 = array();
	$Total6 = array();
	for($i=2; $i<8; $i++){
	
		//global
		$Global6[$i] = $db->pegaUm("select  sum(coalesce(spt.sptunt,0)) as total
								from cte.dimensao d 
								inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' 
								inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' 
								inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A' 
								inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
								inner join cte.acaoindicador ai on ai.ptoid = p.ptoid 
								inner join cte.subacaoindicador sai on sai.aciid = ai.aciid and sai.foaid = 1 and sai.sbaporescola = 'f'
								inner join cte.subacaoparecertecnico spt on spt.sbaid = sai.sbaid and spt.sptano in ('".implode("','",$_REQUEST['chkano'])."') 
								where d.dimstatus = 'A' 
								and d.dimcod = 4
								and ad.ardcod = 1
								and i.indcod = ".$i."
								");
					
		//por escola
		$PorEscola6[$i] = $db->pegaUm("select  sum(coalesce(q.qfaqtd,0)) as total
								from cte.dimensao d 
								inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' 
								inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' 
								inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A' 
								inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
								inner join cte.acaoindicador ai on ai.ptoid = p.ptoid 
								inner join cte.subacaoindicador sai on sai.aciid = ai.aciid and sai.foaid = 1 and (sai.sbaporescola = 't' or sai.sbaporescola is null)
								inner join cte.qtdfisicoano q on q.sbaid = sai.sbaid and q.qfaano in ('".implode("','",$_REQUEST['chkano'])."')
								where d.dimstatus = 'A' 
								and d.dimcod = 4
								and ad.ardcod = 1
								and i.indcod = ".$i."
								");
	
		//pega o total global + por escola
		$Total6[$i] = $Global6[$i] + $PorEscola6[$i];
		
	}
	
	$total_laboratorios_cientificos = $Total6[2] + $Total6[3] + $Total6[4] + $Total6[5] + $Total6[6];
	$total_laboratorios_cientificos_biologia = $Total6[2];
	$total_laboratorios_cientificos_quimica = $Total6[3];
	$total_laboratorios_cientificos_fisica = $Total6[4];
	$total_laboratorios_cientificos_matematica = $Total6[5];
	$total_laboratorios_cientificos_informatica = $Total6[6];
	$total_laboratorios_tecnicos = $Total6[7];
	$total_laboratorios = $total_laboratorios_cientificos + $total_laboratorios_tecnicos;
	
	
	
	
	//7. infra-estrutura
	//379 = construções
	$n_escolas_construcoes = $db->pegaUm("select  count(coalesce(q.entid,0)) as total
								from cte.dimensao d 
								inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' 
								inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' 
								inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A' 
								inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
								inner join cte.acaoindicador ai on ai.ptoid = p.ptoid 
								inner join cte.subacaoindicador sai on sai.aciid = ai.aciid and sai.foaid = 1 and sai.undid = 379
								inner join cte.qtdfisicoano q on q.sbaid = sai.sbaid and q.qfaano in ('".implode("','",$_REQUEST['chkano'])."')
								where d.dimstatus = 'A' 
								and d.dimcod = 4
								and ad.ardcod = 1
								and i.indcod = 1
								");
	
	//384 = ampliações
	$n_escolas_ampliacoes = $db->pegaUm("select  count(coalesce(q.entid,0)) as total
								from cte.dimensao d 
								inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' 
								inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' 
								inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A' 
								inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
								inner join cte.acaoindicador ai on ai.ptoid = p.ptoid 
								inner join cte.subacaoindicador sai on sai.aciid = ai.aciid and sai.foaid = 1 and sai.undid = 384
								inner join cte.qtdfisicoano q on q.sbaid = sai.sbaid and q.qfaano in ('".implode("','",$_REQUEST['chkano'])."')
								where d.dimstatus = 'A' 
								and d.dimcod = 4
								and ad.ardcod = 1
								and i.indcod = 1
								");
	
	//386 = reforma
	$n_escolas_reforma = $db->pegaUm("select  count(coalesce(q.entid,0)) as total
								from cte.dimensao d 
								inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' 
								inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' 
								inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A' 
								inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
								inner join cte.acaoindicador ai on ai.ptoid = p.ptoid 
								inner join cte.subacaoindicador sai on sai.aciid = ai.aciid and sai.foaid = 1 and sai.undid = 386
								inner join cte.qtdfisicoano q on q.sbaid = sai.sbaid and q.qfaano in ('".implode("','",$_REQUEST['chkano'])."')
								where d.dimstatus = 'A' 
								and d.dimcod = 4
								and ad.ardcod = 1
								and i.indcod = 1
								");
	
	//392 = construções ead
	$n_escolas_construcao_ead = $db->pegaUm("select  count(coalesce(q.entid,0)) as total
								from cte.dimensao d 
								inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' 
								inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' 
								inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A' 
								inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
								inner join cte.acaoindicador ai on ai.ptoid = p.ptoid 
								inner join cte.subacaoindicador sai on sai.aciid = ai.aciid and sai.foaid = 1 and sai.undid = 392
								inner join cte.qtdfisicoano q on q.sbaid = sai.sbaid and q.qfaano in ('".implode("','",$_REQUEST['chkano'])."')
								where d.dimstatus = 'A' 
								and d.dimcod = 4
								and ad.ardcod = 1
								and i.indcod = 1
								");
	
	$total_infraestrutura = $n_escolas_construcoes + $n_escolas_ampliacoes + $n_escolas_reforma; 
	
	
	// 8. acervo
	/*
	$PorEscola8 = array();
	$Global8 = array();
	$Total8 = array();
	for($i=1; $i<6; $i++){
	
		//global
		$Global8[$i] = $db->pegaUm("select  sum(coalesce(spt.sptunt,0)) as total
								from cte.dimensao d 
								inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' 
								inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' 
								inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A' 
								inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
								inner join cte.acaoindicador ai on ai.ptoid = p.ptoid 
								inner join cte.subacaoindicador sai on sai.aciid = ai.aciid and sai.foaid = 1 and sai.sbaporescola = 'f'
								inner join cte.subacaoparecertecnico spt on spt.sbaid = sai.sbaid and spt.sptano in ('".implode("','",$_REQUEST['chkano'])."') 
								where d.dimstatus = 'A' 
								and d.dimcod = 4
								and ad.ardcod = 3
								and i.indcod = ".$i."
								");
					
		//por escola
		$PorEscola8[$i] = $db->pegaUm("select  sum(coalesce(q.qfaqtd,0)) as total
								from cte.dimensao d 
								inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' 
								inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' 
								inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A' 
								inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
								inner join cte.acaoindicador ai on ai.ptoid = p.ptoid 
								inner join cte.subacaoindicador sai on sai.aciid = ai.aciid and sai.foaid = 1 and (sai.sbaporescola = 't' or sai.sbaporescola is null)
								inner join cte.qtdfisicoano q on q.sbaid = sai.sbaid and q.qfaano in ('".implode("','",$_REQUEST['chkano'])."')
								where d.dimstatus = 'A' 
								and d.dimcod = 4
								and ad.ardcod = 3
								and i.indcod = ".$i."
								");
	
		//pega o total global + por escola
		$Total8[$i] = $Global8[$i] + $PorEscola8[$i];
		
		$total_acervo = $total_acervo + $Total8[$i];
	}
	*/
	
	//8.1
	$n_compsubacao_area_tecnica = $db->pegaUm("select sum(coalesce(cs.cosqtd,0)) as total
											from cte.dimensao d 
											inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' 
											inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' 
											inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A' 
											inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
											inner join cte.acaoindicador ai on ai.ptoid = p.ptoid 
											inner join cte.subacaoindicador sai on sai.aciid = ai.aciid --and sai.foaid = 1 
											inner join cte.composicaosubacao cs on cs.sbaid = sai.sbaid and cs.cosano in ('".implode("','",$_REQUEST['chkano'])."')
											where d.dimstatus = 'A' 
											and d.dimcod = 4
											and ad.ardcod = 3
											and i.indcod = 5
											");
	
	
	//9. (2008)
	if(in_array('2008', $_REQUEST['chkano'])){
		$vlr_assistencia_financeira_2008 = $db->pegaUm("select sum(coalesce(cs.cosqtd*cs.cosvlruni,0)) as total
													from cte.dimensao d 
													inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' 
													inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' 
													inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A' 
													inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
													inner join cte.acaoindicador ai on ai.ptoid = p.ptoid 
													inner join cte.subacaoindicador sai on sai.aciid = ai.aciid and sai.foaid = 1 and sai.frmid = 17 --(Forma de Execução = Assistência Financeira do MEC )
													inner join cte.composicaosubacao cs on cs.sbaid = sai.sbaid and cs.cosano in ('2008')
													where d.dimstatus = 'A' 
													");
	}
	
	//9. (2009)
	if(in_array('2009', $_REQUEST['chkano'])){
		$vlr_assistencia_financeira_2009 = $db->pegaUm("select sum(coalesce(cs.cosqtd*cs.cosvlruni,0)) as total
													from cte.dimensao d 
													inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' 
													inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' 
													inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A' 
													inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
													inner join cte.acaoindicador ai on ai.ptoid = p.ptoid 
													inner join cte.subacaoindicador sai on sai.aciid = ai.aciid and sai.foaid = 1 and sai.frmid = 17 --(Forma de Execução = Assistência Financeira do MEC )
													inner join cte.composicaosubacao cs on cs.sbaid = sai.sbaid and cs.cosano in ('2009')
													where d.dimstatus = 'A' 
													");
	}
	
	//9. (2010)
	if(in_array('2010', $_REQUEST['chkano'])){
		$vlr_assistencia_financeira_2010 = $db->pegaUm("select sum(coalesce(cs.cosqtd*cs.cosvlruni,0)) as total
													from cte.dimensao d 
													inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' 
													inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' 
													inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A' 
													inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
													inner join cte.acaoindicador ai on ai.ptoid = p.ptoid 
													inner join cte.subacaoindicador sai on sai.aciid = ai.aciid and sai.foaid = 1 and sai.frmid = 17 --(Forma de Execução = Assistência Financeira do MEC )
													inner join cte.composicaosubacao cs on cs.sbaid = sai.sbaid and cs.cosano in ('2010')
													where d.dimstatus = 'A' 
													");
	}
	
	//9. (2011)
	if(in_array('2011', $_REQUEST['chkano'])){
		$vlr_assistencia_financeira_2011 = $db->pegaUm("select sum(coalesce(cs.cosqtd*cs.cosvlruni,0)) as total
													from cte.dimensao d 
													inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' 
													inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' 
													inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A' 
													inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
													inner join cte.acaoindicador ai on ai.ptoid = p.ptoid 
													inner join cte.subacaoindicador sai on sai.aciid = ai.aciid and sai.foaid = 1 and sai.frmid = 17 --(Forma de Execução = Assistência Financeira do MEC )
													inner join cte.composicaosubacao cs on cs.sbaid = sai.sbaid and cs.cosano in ('2011')
													where d.dimstatus = 'A' 
													");
	}
	
	//9. (2008 a 2011)
	$vlr_assistencia_financeira_ate2011 = $db->pegaUm("select sum(coalesce(cs.cosqtd*cs.cosvlruni,0)) as total
													from cte.dimensao d 
													inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' 
													inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' 
													inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A' 
													inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.itrid in (3) and iu.estuf = '".$estuf."' 
													inner join cte.acaoindicador ai on ai.ptoid = p.ptoid 
													inner join cte.subacaoindicador sai on sai.aciid = ai.aciid and sai.foaid = 1 and sai.frmid = 17 --(Forma de Execução = Assistência Financeira do MEC )
													inner join cte.composicaosubacao cs on cs.sbaid = sai.sbaid and cs.cosano in ('2008','2009','2010','2011')
													where d.dimstatus = 'A' 
													");
	
	/*
													select sum(coalesce(cs.cosqtd*cs.cosvlruni,0)) as total
													from cte.dimensao d 
													inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' 
													inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' 
													inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A' 
													inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.estuf = 'BA' 
													inner join cte.acaoindicador ai on ai.ptoid = p.ptoid 
													inner join cte.subacaoindicador sai on sai.aciid = ai.aciid and sai.foaid = 1 and sai.frmid = 17
													inner join cte.composicaosubacao cs on cs.sbaid = sai.sbaid and cs.cosano in ('2008') and cs.sbaid=1420613
													where d.dimstatus = 'A'
													and d.dimcod = 1
													and ad.ardcod = 1
													and i.indcod = 1	
	
	*/
	
	
?>

			<table width=100% cellspacing="0" cellpadding="0" bgcolor="#f5f5f5" border="0" align="center" style="border-bottom: 2px solid black;" >
					<tr><td colspan=7><b>1. Novas vagas de EPT: <b><?if($Total_vagas_EPT) echo $Total_vagas_EPT; else echo "0"; ?></td></tr>
					<tr>
						<td colspan=7>
							<b>1.1 Novas vagas de EMI: <b>
							<?if($Total_vagas_EMI) echo $Total_vagas_EMI; else echo "0"; ?>
							<?if($Total_vagas_EPT){
								$calcp = 0;
								if($Total_vagas_EMI) $calcp = number_format(($Total_vagas_EMI * 100) / $Total_vagas_EPT,2);
								echo " (". $calcp . "% do total de vagas de EPT.)";
							}
							 else{ 
							 	echo " (0%)"; 
							 }
							 ?>
						</td>
					</tr>
					<tr>
						<td colspan=7>
							<table cellspacing="0" cellpadding="0" class="ex">
								<tr style="background-color: #e7e7e7;" >
									<td align="center" width="14%" ><b>Regular</b><br>(1.3.1)</td>
									<td align="center" width="14%" ><b>PROEJA</b><br>(1.3.7)</td>
									<td align="center" width="14%" ><b>PROEP</b><br>(1.3.2)</td>
									<td align="center" width="14%" ><b>Indígenas</b><br>(1.3.3)</td>
									<td align="center" width="14%" ><b>Campo</b><br>(1.3.5)</td>
									<td align="center" width="14%" ><b>Quilombolas / Comunidades Tradicionais</b><br>(1.3.4)</td>
									<td align="center" width="14%" ><b>Jovens em Conflitos <br>com a lei</b><br>(1.3.6)</td>
								</tr>
								<?//php foreach( $acoes as $indice => $acao ): ?>
								<tr style="background-color: <?= $indice % 2 ? '#ffffff' : '#f5f5f5' ?>">
									<td align="center" class="TabelaBorda">
										<?if($Total1[1]) echo $Total1[1]; else echo "0"; ?>
									</td>
									<td align="center">
										<?if($Total1[7]) echo $Total1[7]; else echo "0"; ?>
									</td>
									<td align="center">
										<?if($Total1[2]) echo $Total1[2]; else echo "0"; ?>
									</td>
									<td align="center">
										<?if($Total1[3]) echo $Total1[3]; else echo "0"; ?>
									</td>
									<td align="center">
										<?if($Total1[5]) echo $Total1[5]; else echo "0"; ?>
									</td>
									<td align="center">
										<?if($Total1[4]) echo $Total1[4]; else echo "0"; ?>
									</td>
									<td align="center">
										<?if($Total1[6]) echo $Total1[6]; else echo "0"; ?>
									</td>
								</tr>		
							</table>
						</td>
					</tr>
					
					<tr><td colspan=7>&nbsp;</td></tr>
					<tr>
						<td colspan=7>
							<table width="100%" cellspacing="0" cellpadding="0" class="ex">
								<tr style="background-color: #e7e7e7;">
									<td align="center" width="14%" colspan=2><b>1.2 EAD </b>(1.3.8)</td>
									<td align="center" width="14%" colspan=2><b>1.3 Concomitante </b>(1.3.9)</td>
									<td align="center" width="14%" colspan=3><b>1.4 Subsequente </b>(1.3.10)</td>
								</tr>
								<tr>
									<td align="center" width="14%" colspan=2>
										<?
										if($Total1[8]) 
											echo $Total1[8]; 
										else 
											echo "0"; 
										?>						
									</td>
									<td align="center" width="14%" colspan=2>
										<?
										if($Total1[9]) 
											echo $Total1[9]; 
										else 
											echo "0"; 
										?>
									
									</td>
									<td align="center" width="14%" colspan=3>
										<?
										if($Total1[10]) 
											echo $Total1[10]; 
										else 
											echo "0"; 
										?>
									</td>
								</tr>
								
							</table>
						</td>
					</tr>
					<?if(implode(", ",$_REQUEST['chkano']) == '2008, 2009, 2010, 2011'){?>
					<tr><td colspan=7>&nbsp;</td></tr>
					<tr><td colspan=7>
							<b>O percentual de vagas de EMI está adequado ao propósito do programa?
							<br>
							<?
							//echo $Total_vagas_EPT ." / ". $matriculas_censo ." = ".($Total_vagas_EPT/$matriculas_censo);
							if($matriculas_censo>0){
								if(($Total_vagas_EMI/$matriculas_censo) >= 0.10){
									echo "- SIM (".number_format(($Total_vagas_EMI/$matriculas_censo)*100,2)."% do total EM.)";
									//echo "<BR>CALCULO: (".$Total_vagas_EMI.' / '.$matriculas_censo.') * 100';
								}
								else
									echo "- NÃO (".number_format(($Total_vagas_EMI/$matriculas_censo)*100,2)."% do total EM.)";
							}
							else
								echo "- NÃO ";
							?>
							</b>
						</td>
					</tr>
					<?}?>
					<tr><td colspan=7>&nbsp;</td></tr>
					<tr>
						<td colspan=7><b>2. Professores a serem contratados (1.2.4): </b><?if($Prof_serem_contr_Total) echo $Prof_serem_contr_Total; else echo "0"; ?></td>
					</tr>
					<tr>
						<td colspan=7>
							<table cellspacing="0" cellpadding="0" class="ex" width=100%>
								<tr style="background-color: #e7e7e7;">
									<td align="center" width="50%" colspan=2><b>2.1 Prof. Área Científica</b></td>
									<td align="center" width="50%" colspan=3><b>2.2 Prof. Área Técnologica</b></td>
								</tr>
								<tr>
									<td align="center" width="50%" colspan=2>
										<?
										if($Prof_nucleo_basico_Total) 
											echo ($Prof_nucleo_basico_Total); 
										else 
											echo "0"; 
										?>
									</td>
									<td align="center" width="50%" colspan=2>
										<?
										if($Prof_area_tec_Total) 
											echo $Prof_area_tec_Total; 
										else 
											echo "0"; 
										?>
									
									</td>
								</tr>
							</table>
						</td>
					</tr>
					
					<tr><td colspan=7>&nbsp;</td></tr>
					<!-- 
					<tr><td colspan=7>
							<b>Está adequada na relação para suportar a ampliação de oferta?
							<br>
							<?
							if($Prof_area_tec_Total>0)
								if(($Total_suportar_ampliacao/$Prof_area_tec_Total)>30)
									echo "- NÃO";
								else
									echo "- SIM";
							else
								echo "- SIM";
							?>
							</b>
						</td>
					</tr>
					<tr><td colspan=7>&nbsp;</td></tr>
					 -->
					<tr>
						<td colspan=7><b>3. Profissionais da educação capacitados </b></td>
					</tr>
					<tr>
						<td colspan=7>
							<table cellspacing="0" cellpadding="0" class="ex" width="100%">
								<tr style="background-color: #e7e7e7;">
									<td align="center" width="34%" colspan=2><b>3.1 Professores</b></td>
									<td align="center" width="33%" colspan=2><b>3.1.1 Formação inicial</b> (2.1.1 - 2.1.5)</td>
									<td align="center" width="33%" colspan=3><b>3.1.2 Formação continuada</b> (2.2.1; 2.2.3 - 2.1.5)</td>
								</tr>
								<tr>
									<td align="center" width="34%" colspan=2>
										<b>
										<?
										if($prof_cap_inicial || $prof_cap_continuada) 
											echo ($prof_cap_inicial + $prof_cap_continuada); 
										else 
											echo "0"; 
										?>
										</b>						
									</td>
									<td align="center" width="33%" colspan=2>
										<?
										if($prof_cap_inicial) 
											echo $prof_cap_inicial; 
										else 
											echo "0"; 
										?>
									
									</td>
									<td align="center" width="33%" colspan=3>
										<?
										if($prof_cap_continuada) 
											echo $prof_cap_continuada; 
										else 
											echo "0"; 
										?>
									</td>
								</tr>
							</table>
						</td>
					</tr>
					<tr><td colspan=7>&nbsp;</td></tr>
					<tr>
						<td colspan=7>
							<table cellspacing="0" cellpadding="0" class="ex" width="100%">
					
								<tr style="background-color: #e7e7e7;">
									<td align="center" width="34%" colspan=2><b>3.2 Equipes Pedagógicas</b></td>
									<td align="center" width="33%" colspan=2><b>3.2.1 Inicial</b> (2.1.6)</td>
									<td align="center" width="33%" colspan=3><b>3.2.2 Continuada</b> (2.2.2)</td>
								</tr>
								<tr>
									<td align="center" width="34%" colspan=2>
										<b>
										<?
										if($equip_pedagog_cap_inicial || $equip_pedagog_cap_continuada) 
											echo ($equip_pedagog_cap_inicial + $equip_pedagog_cap_continuada); 
										else 
											echo "0"; 
										?>
										</b>
									</td>
									<td align="center" width="33%" colspan=2>
										<?
										if($equip_pedagog_cap_inicial) 
											echo $equip_pedagog_cap_inicial;
										else
											echo "0"; 
										?>
									</td>
									<td align="center" width="33%" colspan=3>
										<?
										if($equip_pedagog_cap_continuada) 
											echo $equip_pedagog_cap_continuada;
										else
											echo "0"; 
										?>
									</td>
								</tr>	
							</table>
						</td>
					</tr>
												
					<tr><td colspan=7>&nbsp;</td></tr>
					<tr>
						<td colspan=7>
							<table cellspacing="0" cellpadding="0" class="ex" width="100%">
					
								<tr style="background-color: #e7e7e7;">
									<td align="left" colspan=7><b>&nbsp;3.3 Técnicos de laboratórios</b> (4.2.8)</td>
								</tr>
								<tr>
									<td align="left" colspan=7>
										&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
										&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
										&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
										&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
										&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
										&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
										&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
										<?
										if($tec_laboratorio_cap) 
											echo $tec_laboratorio_cap;
										else
											echo "0"; 
										?>
									</td>
								</tr>
							</table>
						</td>
					</tr>
								
					<tr><td colspan=7>&nbsp;</td></tr>
					<tr>
						<td colspan=7><b>4. Escolas e Projetos Políticos Pedagógicos (PPP)</b></td>
					</tr>
					<tr>
						<td colspan=7><b>4.1 Número total de escolas: <?if($total_escolas_atendidas) echo $total_escolas_atendidas; else echo "0"; ?></b></td>
					</tr>
					<tr>
						<td colspan=7>
							<table cellspacing="0" cellpadding="0" class="ex" width="100%">
					
								<tr style="background-color: #e7e7e7;">
									<td align="center" width="50%" colspan=3><b>Somente com Ensino Médio</b></td>
									<td align="center" width="50%" colspan=4><b>Com PPPs Preenchidos</b></td>
								</tr>
								<tr>
									<td align="center" width="50%" colspan=3>
										<?
										if($total_escolas_sem_ept) 
											echo $total_escolas_sem_ept;
										else
											echo "0"; 
										?>
									</td>
									<td align="center" width="50%" colspan=4>
										<?
										if($porcentagem_ppps_sem_ept) 
											echo number_format($porcentagem_ppps_sem_ept,2). " %";
										else
											echo "0 %"; 
										?>
									</td>
								</tr>
							</table>
						</td>
					</tr>
					<tr><td colspan=7>&nbsp;</td></tr>
					<tr>
						<td colspan=7>
							<table cellspacing="0" cellpadding="0" class="ex" width="100%">
					
								<tr style="background-color: #e7e7e7;">
									<td align="center" width="50%" colspan=3><b>Com EPT</b></td>
									<td align="center" width="50%" colspan=4><b>Com PPPs Preenchidos</b></td>
								</tr>
								<tr>
									<td align="center" width="50%" colspan=3>
										<?
										if($total_escolas_com_ept) 
											echo $total_escolas_com_ept;
										else
											echo "0"; 
										?>
									</td>
									<td align="center" width="50%" colspan=4>
										<?
										if($porcentagem_ppps_com_ept) 
											echo number_format($porcentagem_ppps_com_ept,2). " %";
										else
											echo "0 %"; 
										?>
									</td>
								</tr>
							</table>
						</td>
					</tr>
					<tr><td colspan=7>&nbsp;</td></tr>
					<tr>
						<td colspan=7><b>4.2 Data Final para Finalização dos PPPs: </b> 30/11/2008</td>
					</tr>
					<tr><td colspan=7>&nbsp;</td></tr>
					<tr>
						<td colspan=7><b>4.3 Cursos mais Solicitados:</b></td>
					</tr>
					<tr>
						<td colspan=7>
							<table cellspacing="0" cellpadding="0" class="ex" width="100%">
					
								<tr style="background-color: #e7e7e7;">
									<td align="center" width="50%" colspan=3><b>Cursos </b></td>
									<td align="center" width="50%" colspan=4><b>Qtd.</b></td>
								</tr>
								<?
								if($lista_cursos_mais_solicitados){
								 foreach ($lista_cursos_mais_solicitados as $curso) {								
								?>
									<tr>
										<td align="left" width="50%" colspan=3>
											&nbsp;<?=$curso['nome'];?>
										</td>
										<td align="center" width="50%" colspan=4>
											<?=$curso['qtd'];?>
										</td>
									</tr>
								<?}
								}?>
							</table>
						</td>
					</tr>
					<tr><td colspan=7>&nbsp;</td></tr>
					<tr>
						<td colspan=7><b>4.4 Escolas Estaduais Financiadas pelo PROEP (1.3.2): </b><?if($Escolas_finan_PROEP) echo $Escolas_finan_PROEP; else echo "0"; ?></td>
					</tr>
					<tr><td colspan=7>&nbsp;</td></tr>
					<tr>
						<td colspan=7><b>4.4.1 Escolas do PROEP do segmento comunitário que poderão ser absorvidas pelo Estado </b> </td>
					</tr>
					<tr>
						<td colspan=7>
							<table cellspacing="0" cellpadding="3" class="ex" width="100%">
								<tr style="background-color: #e7e7e7;">
									<td align="center" width="50%" colspan=3><b>Escola</b></td>
									<td align="center" width="50%" colspan=4><b>Parecer</b></td>
								</tr>
								<?
								if($escolas_proep_parecer){
								 foreach ($escolas_proep_parecer as $escolas_proep_parecer) {								
								?>
									<tr>
										<td align="left" width="50%" colspan=3>
											&nbsp;<?=$escolas_proep_parecer["nome"];?>
										</td>
										<td align="left" width="50%" colspan=4>
											<?=$escolas_proep_parecer["parecer"];?>
										</td>
									</tr>
								<?}
								}?>
							</table>
						</td>
					</tr>
					<tr><td colspan=3></td><td colspan=4></td></tr>
					<tr><td colspan=3></td><td colspan=4></td></tr>
					<tr><td colspan=3></td><td colspan=4></td></tr>
					<tr><td colspan=7>&nbsp;</td></tr>
					<tr>
						<td colspan=7><b>5. OFERTA DE EAD (ETEC-Brasil) </b></td>
					</tr>
					<tr>
						<td colspan=7>
							<table cellspacing="0" cellpadding="0" class="ex" width="100%">
								<tr style="background-color: #e7e7e7;">
									<td align="center" width="33%" colspan=2><b>5.1. Números de Núcleos de EaD</b> (4.2.4)</td>
									<td align="center" width="33%" colspan=2><b>5.2. Números de Pólos de EaD</b> (1.3.8)</td>
									<td align="center" width="33%" colspan=3><b>5.3. Números de Salas de Apoio Presencial e EaD</b> (4.2.1)</td>
								</tr>
								<tr>
									<td align="center" width="33%" colspan=2>
										<?
										if($n_escolas_ead) 
											echo $n_escolas_ead;
										else
											echo "0"; 
										?>
									</td>
									<td align="center" width="33%" colspan=2>
										<?
										if($n_polos_ead) 
											echo $n_polos_ead; 
										else 
											echo "0"; 
										?>
									</td>
									<td align="center" width="33%" colspan=3>
										<?
										if($Total5[1]) 
											echo $Total5[1]; 
										else 
											echo "0"; 
										?>
									</td>
								</tr>
							</table>
						</td>
					</tr>
					<tr><td colspan=7>&nbsp;</td></tr>
					<!-- 
					<tr>
						<td colspan=7>
							<table cellspacing="0" cellpadding="0" class="ex" width="100%">
					
								<tr style="background-color: #e7e7e7;">
									<td align="left" colspan=7><b>5.1. Cursos </b></td>
								</tr>
								<?
								//if($lista_cursos_ead){
								// foreach ($lista_cursos_ead as $cursoead) {	
								?>
									<tr>
										<td align="left" colspan=7>
											<?//=str_replace ( "Alunos de Ead no Curso" , "" , $cursoead['nome'] );?>
										</td>
									</tr>
								<?//}
								//}?>
							</table>
						</td>
					</tr>
					<tr><td colspan=7>&nbsp;</td></tr>
					 -->
					<tr>
						<td colspan=7><b>6. Laboratórios: </b><?if($total_laboratorios) echo $total_laboratorios; else echo "0"; ?> </td>
					</tr>
					<tr>
						<td colspan=7>
							<table cellspacing="0" cellpadding="0" class="ex" width="100%">
								<tr style="background-color: #e7e7e7;">
									<td align="center" width="50%" colspan=3><b>6.1 Científicos</b> (4.1.2 - 4.1.6)</td>
									<td align="center" width="50%" colspan=4><b>6.2 Tecnológicos</b> (4.2.7)</td>
								</tr>
								<tr>
									<td align="center" width="50%" colspan=3>
										<?
										if($total_laboratorios_cientificos) 
											echo $total_laboratorios_cientificos;
										else
											echo "0"; 
										?>
									</td>
									<td align="center" width="50%" colspan=4 rowspan=3>
										<?
										if($total_laboratorios_tecnicos) 
											echo $total_laboratorios_tecnicos;
										else
											echo "0"; 
										?>
									</td>
								</tr>
								<tr>
									<td align="center" width="50%" colspan=3>
										Biologia(<?=$total_laboratorios_cientificos_biologia?>)
										&nbsp;&nbsp;
										Química(<?=$total_laboratorios_cientificos_quimica?>)
										&nbsp;&nbsp;
										Física(<?=$total_laboratorios_cientificos_fisica?>)
									</td>
									<td align="center" width="50%" colspan=4>
									</td>
								</tr>
								<tr>
									<td align="center" width="50%" colspan=3>
										Matemática(<?=$total_laboratorios_cientificos_matematica?>)
										&nbsp;&nbsp;
										Informática(<?=$total_laboratorios_cientificos_informatica?>)
									</td>
									<td align="center" width="50%" colspan=4>
									</td>
								</tr>

							</table>
						</td>
					</tr>
					<tr><td colspan=7>&nbsp;</td></tr>
					<tr>
						<td colspan=7><b>7. Infra-Estrutura (4.1.1): </b><?if($total_infraestrutura) echo $total_infraestrutura; else echo "0";?> </td>
					</tr>
					<tr>
						<td colspan=7>
							<table cellspacing="0" cellpadding="0" class="ex" width="100%">
								<tr style="background-color: #e7e7e7;">
									<td align="center" width="25%" colspan=1><b>7.1 Construções de novas escolas de EMI</b></td>
									<td align="center" width="25%" colspan=2><b>7.2 Ampliações</b></td>
									<td align="center" width="25%" colspan=2><b>7.3 Reforma</b></td>
									<td align="center" width="25%" colspan=2><b>7.4 Construção de Núcleos</b></td>
								</tr>
								<tr>
									<td align="center" width="25%" colspan=1>
										<?
										if($n_escolas_construcoes) 
											echo $n_escolas_construcoes;
										else
											echo "0"; 
										?>
									</td>
									<td align="center" width="25%" colspan=2>
										<?
										if($n_escolas_ampliacoes) 
											echo $n_escolas_ampliacoes;
										else
											echo "0"; 
										?>
									</td>
									<td align="center" width="25%" colspan=2>
										<?
										if($n_escolas_reforma) 
											echo $n_escolas_reforma;
										else
											echo "0"; 
										?>
									</td>
									<td align="center" width="25%" colspan=2>
										<?
										if($n_escolas_construcao_ead) 
											echo $n_escolas_construcao_ead;
										else
											echo "0"; 
										?>
									</td>

								</tr>
							</table>
						</td>
					</tr>
					<tr><td colspan=7>&nbsp;</td></tr>
					<tr>
						<td colspan=7>
							<b>8. Acervo (Livros)</b>
							&nbsp;
							<?
							/*
							if($total_acervo) 
								echo $total_acervo;
							else
								echo "0";
							*/ 
							?>
						</td>
					</tr>
					<tr>
						<td colspan=7>
							<table cellspacing="0" cellpadding="0" class="ex" width="100%">
								<tr style="background-color: #e7e7e7;">
									<td colspan=7 style="padding-left:110px"><b>8.1 Área Tecnológica</b> (4.3.5)</td>
								</tr>
								<tr>
									<td olspan=7 style="padding-left:160px">
										<?
										//$at = $Total8[5];
										$at = $n_compsubacao_area_tecnica;
										if($at) 
											echo (float) $at;
										else
											echo "0"; 
										?>
									</td>
								</tr>
							</table>
						</td>
					</tr>
					<tr><td colspan=7>&nbsp;</td></tr>
					<tr>
						<td colspan=7><b>9. Recursos Solicitados para Assistência Financeira do MEC</b> </td>
					</tr>
					<tr>
						<td colspan=7>
							<table cellspacing="0" cellpadding="0" class="ex" width="100%">
								<tr style="background-color: #e7e7e7;">
									<?if(in_array('2008', $_REQUEST['chkano'])){ ?>
									<td align="center" colspan=2>
										<b>Valor em 2008</b>
									</td>
									<?}?>
									<?if(in_array('2009', $_REQUEST['chkano'])){ ?>
									<td align="center" colspan=2>
										<b>Valor em 2009</b>
									</td>
									<?}?>
									<?if(in_array('2010', $_REQUEST['chkano'])){ ?>
									<td align="center" colspan=2>
										<b>Valor em 2010</b>
									</td>
									<?}?>
									<?if(in_array('2011', $_REQUEST['chkano'])){ ?>
									<td align="center" colspan=2>
										<b>Valor em 2011 </b>
									</td>
									<?}?>
									<td align="center" colspan=2>
										<b>Valor 2008 à 2011</b>
									</td>
								</tr>
								<tr>
									<?if(in_array('2008', $_REQUEST['chkano'])){ ?>
									<td align="center" colspan=2>
										<?
										if($vlr_assistencia_financeira_2008) 
											echo "R$ ".number_format($vlr_assistencia_financeira_2008,2,',','.');
										else
											echo "R$ 0,00";
										
										?>
									</td>
									<?}?>
									<?if(in_array('2009', $_REQUEST['chkano'])){ ?>
									<td align="center" colspan=2>
										<?
										if($vlr_assistencia_financeira_2009) 
											echo "R$ ".number_format($vlr_assistencia_financeira_2009,2,',','.');
										else
											echo "R$ 0,00";
										
										?>
									</td>
									<?}?>
									<?if(in_array('2010', $_REQUEST['chkano'])){ ?>
									<td align="center" colspan=2>
										<?
										if($vlr_assistencia_financeira_2010) 
											echo "R$ ".number_format($vlr_assistencia_financeira_2010,2,',','.');
										else
											echo "R$ 0,00";
										
										?>
									</td>
									<?}?>
									<?if(in_array('2011', $_REQUEST['chkano'])){ ?>
									<td align="center" colspan=2>
										<?
										if($vlr_assistencia_financeira_2011) 
											echo "R$ ".number_format($vlr_assistencia_financeira_2011,2,',','.');
										else
											echo "R$ 0,00";
										
										?>
									</td>
									<?}?>
									<td align="center" colspan=2>
										<?
										if($vlr_assistencia_financeira_ate2011) 
											echo "R$ ".number_format($vlr_assistencia_financeira_ate2011,2,',','.');
										else
											echo "R$ 0,00";
										
										?>
									</td>
								</tr>
							</table>
						</td>
					</tr>
					<tr><td colspan=7>&nbsp;</td></tr>
			</table>



<?}?>

</form>

</body>
</html>