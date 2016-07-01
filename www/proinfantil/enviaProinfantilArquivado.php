<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/workflow.php";
include_once "_constantes.php";
 
session_start();

//$_SESSION['usucpf'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "SELECT 
	        pi.pinid,
	        upper(ee.entnome) as Nome_Instituicao,
	        upper(oi.obrnome) as Nome_Da_Obra,
	        ed.estuf as UF,
	        tm.mundescricao as municipio,
	        wok.esddsc as descricaowork,
	        date_part('year', cast(dt.resdsc as date)) as anoini,
	        wok.usucpf,
	        wok.hstid,
	        pi.docid
	    FROM
	        obras2.obras AS oi
	        INNER JOIN obras2.empreendimento e ON e.empid =  oi.empid
	        INNER JOIN entidade.entidade AS ee ON oi.entid= ee.entid
	        INNER JOIN entidade.endereco AS ed ON oi.endid = ed.endid
	        INNER JOIN territorios.municipio AS tm ON ed.muncod = tm.muncod
	        inner JOIN proinfantil.proinfantil pi ON pi.obrid = oi.obrid
	        LEFT JOIN (SELECT 		q.pinid, r.resdsc
	                   FROM 		proinfantil.questionario q
	                   INNER JOIN 	questionario.resposta r on r.qrpid = q.qrpid
	                   WHERE	 	r.perid = 1587 ) as dt ON dt.pinid = pi.pinid				
	        /*inner JOIN workflow.documento d ON d.docid = pi.docid
	        inner JOIN workflow.estadodocumento edoc on edoc.esdid = d.esdid
	        inner join workflow.historicodocumento hd on hd.docid = d.docid and hd.aedid = 2227 and hd.usucpf = ''*/
	        inner join (select
	                        d.docid,
	                        ed.esddsc,
	                        hd.htddata,
	                        hd.usucpf,
	                        hd.hstid,
	                        (select e.esddsc from workflow.estadodocumento e where e.esdid = ac.esdidorigem) as origem,
	                        (select e.esddsc from workflow.estadodocumento e where e.esdid = ac.esdiddestino) as destino
	                    from
	                        workflow.documento d 
	                        inner JOIN workflow.estadodocumento ed on ed.esdid = d.esdid
	                        inner join workflow.historicodocumento hd on hd.docid = d.docid
	                        inner join workflow.acaoestadodoc ac on ac.aedid = hd.aedid
	                    where
	                        d.tpdid = 48
	                        and d.esdid = 517
	                        and hd.hstid in (select max(h.hstid) from workflow.historicodocumento h
	                                            where h.usucpf = '' and h.aedid = 2227 and h.docid = d.docid)
	               ) as wok on wok.docid = pi.docid
	    WHERE
	        dt.resdsc is not null
	    ORDER BY
	        ee.entnome";

$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();

//aedid=2227 #Encaminhar para Indeferido Arquivado
//esdid=517 #Indeferido arquivado

//aedid=2578 #Encaminhar para Indeferido Arquivado / Sistema
//esdid=1106 #Indeferido arquivado / Sistema

$anoIAS = array('2013', '2014');
$anoIA = array('2011', '2012');
$countIAS = 0;
$countIA = 0;
foreach ($arrDados as $v) {
	if( in_array($v['anoini'], $anoIAS) ){
		$countIAS++;
		$boTem = $db->pegaUm("select count(praid) from proinfantil.proinfanciaanalise where pinid = {$v['pinid']} and prastatus = 'A'");
		
		if( $boTem > 0 ){
			$sql = "update proinfantil.proinfanciaanalise set prastatus = 'I' where pinid = {$v['pinid']}";
			$db->executar($sql);
		}
		$parecer = '<p>Informamos que o pleito foi indeferido por decurso de prazo, pois de acordo com o estabelecido no &sect;4&ordm; Art. 5&ordm; da Resolu&ccedil;&atilde;o CD/FNDE n&ordm; 15, transcrito abaixo, o munic&iacute;pio n&atilde;o respondeu a dilig&ecirc;ncia em tempo h&aacute;bil.</p>
					<p>Resolu&ccedil;&atilde;o CD/FNDE n&ordm; 15, Art. 5&ordm;, &sect;4&ordm;:</p>
					<p><em>&sect; 4&ordm; O munic&iacute;pio ou o DF ter&aacute; o prazo m&aacute;ximo de 90 (noventa) dias para esclarecera SEB/MEC sobre os estabelecimentos cuja situa&ccedil;&atilde;o seja apresentada no Simec como "em dilig&ecirc;ncia".</em></p>
					<p>Coordena&ccedil;&atilde;o Geral de Educa&ccedil;&atilde;o Infantil</p>';
		
		$sql = "INSERT INTO proinfantil.proinfanciaanalise(prapareceraprovacao, prastatus, pradata, usucpf, pinid, praanoanalise, praarquivada) 
				VALUES ('{$parecer}', 'A', now(), '', {$v['pinid']}, 1, true)";
		$db->executar($sql);
		
		if( $v['docid'] ){
			$sql = "update workflow.documento d set esdid = 1106 where docid = ".$v['docid'];
			$db->executar($sql);
			
			$sql = "update workflow.historicodocumento d set aedid = 2578 where hstid = ".$v['hstid'];
			$db->executar($sql);
		}
	}
	
	if( in_array($v['anoini'], $anoIA) ){
		$countIA++;
		
		$boTem = $db->pegaUm("select count(praid) from proinfantil.proinfanciaanalise where pinid = {$v['pinid']} and prastatus = 'A'");
		
		if( $boTem > 0 ){
			$sql = "update proinfantil.proinfanciaanalise set prastatus = 'I' where pinid = {$v['pinid']}";
			$db->executar($sql);
		}
		$parecer = 'As informações sobre o estabelecimento, inseridas no Sistema Integrado de Monitoramento, Execução e Controle do Ministério da Educação (Simec), para recebimento de recursos financeiros para apoio à manutenção de novos estabelecimentos de educação infantil públicos, construídos com recursos federais, foram analisadas conforme critérios estabelecidos pela Resolução CD/FNDE nº 52 de 29 de setembro de 2011. Informamos que o pleito foi indeferido por decurso de prazo, pois a) o município não respondeu a diligência em tempo hábil; b) de acordo com o que estabelece o Art. 5º da Lei nº 12.499, transcrito abaixo, o município teve tempo hábil para informar no Censo Escolar da Educação Básica as matrículas do estabelecimento a fim de repasse de recursos do Fundeb. Lei nº 12.499, Art. 5º: Art. 5º Os novos estabelecimentos de educação infantil de que trata o art. 1º deverão ser cadastrados por ocasião da realização do Censo Escolar imediatamente após o início das atividades escolares, sob pena de interrupção do apoio financeiro e devolução das parcelas já recebidas. Coordenação Geral de Educação Infantil';
		
		$sql = "INSERT INTO proinfantil.proinfanciaanalise(prapareceraprovacao, prastatus, pradata, usucpf, pinid, praanoanalise, praarquivada) 
				VALUES ('{$parecer}', 'A', now(), '', {$v['pinid']}, 1, true)";
		$db->executar($sql);
	}
}
$db->commit();
ver($countIAS, $countIA);
?>