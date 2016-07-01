<?php
include APPRAIZ ."includes/cabecalho.inc";
print '<br>';

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Listar Demandas', '&nbsp;' );

$sql = "SELECT
 DISTINCT
	lpad(cast(d.dmdid as varchar),
				case when length(cast(d.dmdid as varchar)) > 6 then 
						length(cast(d.dmdid as varchar)) 
				else 
					6 
				end 
				, '0') as id, 
(CASE p.priid
    WHEN 3 THEN '<img src=\'../imagens/pd_urgente.JPG\' />'|| ' ' || p.pridsc
    WHEN 1 THEN '<img src=\'../imagens/pd_normal.JPG\' />'|| ' ' || p.pridsc
    WHEN 2 THEN '<img src=\'../imagens/pd_alta.JPG\' />'|| ' ' || p.pridsc
    ELSE '<div style=\'color:red;\' title=\'Não Atribuído\'>N/A</div>'
 END) AS prioridade,
 
 (CASE 
	WHEN dmdidpausa > 0 AND ( dttempopausa is null OR dttempopausa > to_char(CURRENT_TIMESTAMP,'YYYY-MM-DD HH24:MI') ) THEN
		'<img src=\'../imagens/pause.gif\' border=0  title=\' \' align=\'absmiddle\'  onmouseout=\'SuperTitleOff(this);\' onmousemove=\"SuperTitleAjax(\'demandas.php?modulo=principal/lista&acao=A&dmdidPausaAjax='|| d.dmdid ||'\',this);\">'
	ELSE
		''	
 END
 ||					 	
 CASE 
	WHEN dm.contador > 0 THEN 
		'<img id=\'img_mais_'|| d.dmdid ||'\' src=\'../imagens/mais.gif\' border=\'0\'><img id=\'img_menos_'|| d.dmdid ||'\' src=\'../imagens/menos.gif\' border=\'0\' style=\'display:none\'> ' || d.dmdtitulo
	ELSE
		d.dmdtitulo
 END) AS tit,
 
 CASE WHEN esddsc in ('Finalizada','Validada','Validada sem pausa','Invalidada','Aguardando validação') THEN
		dmddsc
	ELSE
		(CASE WHEN
			d.dmddatafimprevatendimento < CURRENT_DATE
		 THEN '<font color=\"red\">' || dmddsc || '</font>'
		 ELSE '<font color=\"green\">' || dmddsc || '</font>'
		 END)
 END as descricao,
 CASE
  WHEN d.docid IS NOT NULL THEN 
  
  CASE esddsc WHEN 'Cancelada' THEN '<span style=\'color:red;\'>' || esddsc || '</span>'
	ELSE  esddsc
  END
  
  ELSE '<span style=\'color:blue;\' title=\'Em Processamento\'>Em Processamento</span>'
 END AS situacao,
 CASE 
	WHEN d.dmdnomedemandante != '' THEN upper(d.dmdnomedemandante)
	ELSE upper(u.usunome)
  END as usuario,
  CASE
	WHEN u2.usucpf != '' THEN upper(u2.usunome)
	ELSE '<span style=\'color:red;\' title=\'Não Atribuído\'>N/A</span>'
  END AS responsavel,					 
 CASE
	WHEN u2.usucpf != '' THEN upper(u2.usunome)
	ELSE '<span style=\'color:red;\' title=\'Não Atribuído\'>N/A</span>'
  END AS responsavel,
  '<span style=\"display:none\">' || d.dmddatainclusao || '</span>' ||
 CASE 
	WHEN dm.contador > 0 THEN 
	  to_char(d.dmddatainclusao, 'DD-MM-YYYY HH24:MI:SS') 
	ELSE 
	  to_char(d.dmddatainclusao, 'DD-MM-YYYY HH24:MI:SS')
 END AS datainclusao,
 to_char(d.dmddatainiprevatendimento, 'DD-MM-YYYY HH24:MI:SS') AS dataprevisaoinicio,
 CASE 
	WHEN dm.contador > 0 
	THEN
		(CASE esddsc 
			WHEN 'Validada' THEN
				'<font title=\"Demanda Validada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font> </tr><tr style=\"background-color:#F7F7F7\" ><td colspan=11 style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
			WHEN 'Validada sem pausa' THEN
				'<font title=\"Demanda Validada sem pausa!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font> </tr><tr style=\"background-color:#F7F7F7\" ><td colspan=11 style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
			WHEN 'Invalidada' THEN
				'<font title=\"Demanda Invalidada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font> </tr><tr style=\"background-color:#F7F7F7\" ><td colspan=11 style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
			WHEN 'Finalizada' THEN
				'<font title=\"Demanda finalizada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font> </tr><tr style=\"background-color:#F7F7F7\" ><td colspan=11 style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
			WHEN 'Aguardando validação' THEN
				'<font title=\"Demanda Aguardando validação!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font> </tr><tr style=\"background-color:#F7F7F7\" ><td colspan=11 style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
		 ELSE
			(CASE WHEN d.dmddatafimprevatendimento is not null THEN
				(CASE WHEN to_char(d.dmddatafimprevatendimento::date,'YYYY-MM-DD') = to_char(CURRENT_DATE::date,'YYYY-MM-DD')
					THEN '<font color=\"#FBB917\" title=\"Demanda com vencimento hoje!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font>' || '</tr><tr style=\"background-color:#F7F7F7\" ><td colspan=11 style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
					ELSE 
						(CASE WHEN d.dmddatafimprevatendimento < CURRENT_DATE
							THEN '<font color=\"red\" title=\"Demanda em atraso!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font>' || '</tr><tr style=\"background-color:#F7F7F7\" ><td colspan=11 style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
							ELSE '<font color=\"green\" title=\"Demanda em dia!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font>' || '</tr><tr style=\"background-color:#F7F7F7\" ><td colspan=11 style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
						END)						 		
				END)						 		
			ELSE '</tr><tr style=\"background-color:#F7F7F7\" ><td colspan=11 style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
			END)
		 END)
	ELSE
		(CASE esddsc 
			WHEN 'Validada' THEN
				'<font title=\"Demanda Validada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
			WHEN 'Validada sem pausa' THEN
				'<font title=\"Demanda Validada sem pausa!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
			WHEN 'Invalidada' THEN
				'<font title=\"Demanda Invalidada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
			WHEN 'Finalizada' THEN
				'<font title=\"Demanda Finalizada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
			WHEN 'Aguardando validação' THEN
				'<font title=\"Demanda Aguardando validação!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
		 ELSE
			(CASE WHEN d.dmddatafimprevatendimento is not null THEN
				(CASE WHEN to_char(d.dmddatafimprevatendimento::date,'YYYY-MM-DD') = to_char(CURRENT_DATE::date,'YYYY-MM-DD')
					THEN '<font color=\"#FBB917\" title=\"Demanda com vencimento hoje!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font>' 
					ELSE 
						(CASE WHEN d.dmddatafimprevatendimento < CURRENT_DATE
							THEN '<font color=\"red\" title=\"Demanda em atraso!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font>' 
							ELSE '<font color=\"green\" title=\"Demanda em dia!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font>' 
						END)						 		
				END)						 		
			END)
		 END)
 END AS dataprevisaotermino 
 FROM
 demandas.demanda d
 LEFT JOIN ( select dmdid, count(ihdid) as ctnseriepatr from demandas.itemhardwaredemanda group by dmdid ) nsp ON nsp.dmdid = d.dmdid
 LEFT JOIN ( select dmdidorigem,  count(dmdid) as contador from demandas.demanda group by dmdidorigem ) dm ON dm.dmdidorigem = d.dmdid 
 --pega subdemandas com pausa
 LEFT JOIN ( select pp.dmdid, count(pp.dmdid) as dmdidpausa, to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI') as dttempopausa 
			 from demandas.pausademanda pp
			 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
			 inner join workflow.documento doc2 ON doc2.docid = dd.docid 
			 where pp.pdmstatus = 'A' and doc2.esdid in (91,92,107,108)
			 group by dd.dmddatafimprevatendimento,pp.dmdid) ps ON ps.dmdid = d.dmdid
 --pega historico da demanda
 LEFT JOIN ( select dmdid, count(dmdid) as dmdidpausahist from demandas.pausademanda where pdmstatus = 'A' group by dmdid ) phst ON phst.dmdid = d.dmdid			 
 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
 LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
 LEFT JOIN demandas.prioridade p ON p.priid = d.priid					 
 LEFT JOIN workflow.documento doc ON doc.docid = d.docid
 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid
 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
 LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
 WHERE 
 dmdstatus = 'A'	
 AND d.sidid = 42 AND ( doc.esdid not in (95,100,109,110,135,136,170) or doc.esdid is null )		 
 ORDER BY id DESC";

$arrCabecalho = array("Cód","Prioridade","Assunto","Descrição da Atividade","Situação","Solicitante","Técnico Responsável","Data de Abertura","Data prevista de início","Data prevista de término");

$db->monta_lista( $sql, $arrCabecalho, 50, 10, 'N', '', '' );
?>