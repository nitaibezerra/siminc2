<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes/RequestHttp.class.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once "../_funcoesPar.php";

session_start();

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "select distinct
			pp.prpid,
			pp.prpnumeroprocesso,
		    dp.dopid,
		    (array_to_string(array(select p.sbdid from par.processoparcomposicao p where p.prpid = pp.prpid and p.ppcstatus = 'A'), ', ')) as subacao
		from
			par.processopar pp
		    inner join par.processoparcomposicao ppc on ppc.prpid = pp.prpid and ppc.ppcstatus = 'A'
		    inner join par.vm_documentopar_ativos dp on dp.prpid = pp.prpid
		    inner join par.subacaodetalhe sd on sd.sbdid = ppc.sbdid
		    inner join par.subacao s on s.sbaid = sd.sbaid
		    inner join par.modelosdocumentos md on md.mdoid = dp.mdoid and md.mdostatus = 'A'
		where
			pp.prpstatus = 'A'
		    --and pp.inuid = 1
		    and dp.arqid is null
		    and s.sbacronograma = 2
		    and md.tpdcod in (21, 102, 103)";

$arrProcesso = $db->carregar($sql);
$arrProcesso = $arrProcesso ? $arrProcesso : array();

foreach ($arrProcesso as $v) {
	if( $v['subacao'] ){
		$sqlRelatorio = "SELECT * FROM (
	                    SELECT
	                        CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE mun.estuf END as uf,
	                        CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE mun.mundescricao END as entidade,
	                        ent.entnome as escola,
	                        ent.entcodent as codinep,
	                        (par.retornacodigosubacao(sd.sbaid)) as subacao,
	                        pic.picdescricao as item,
	                        CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 )
	                            THEN -- escolas sem itens
	                                sum(coalesce(se.sesquantidadetecnico,0) * coalesce(sic.icovalor,0))::numeric(20,2)
	                            ELSE -- escolas com itens
	                                CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
	                                    sum(ssi.seiqtdtecnico)
	                                END
	                        END as quantidade 
	                    FROM 
	                        par.subacaodetalhe sd 
		                    INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
		                    INNER JOIN par.subacaoitenscomposicao sic ON sic.sbaid = sd.sbaid AND sic.icoano = sd.sbdano
		                    INNER JOIN par.propostaitemcomposicao pic ON pic.picid = sic.picid
		                    INNER JOIN par.subacaoescolas se ON se.sbaid = sd.sbaid AND se.sesano = sd.sbdano
		                    INNER JOIN par.escolas esc on esc.escid = se.escid
		                    INNER JOIN entidade.entidade ent ON ent.entid = esc.entid
		                    INNER JOIN par.subescolas_subitenscomposicao ssi ON ssi.icoid = sic.icoid AND ssi.sesid = se.sesid
		                    INNER JOIN par.acao a ON a.aciid = s.aciid
		                    INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid
		                    INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
		                    LEFT JOIN territorios.municipio mun ON mun.muncod = iu.muncod
	                    WHERE 
	                        sd.sbdid IN (".$v['subacao'].")
	                    GROUP BY
	                        ent.entnome, ent.entcodent, sd.sbaid, pic.picdescricao, s.frmid, s.ptsid, sic.icovalidatecnico, iu.itrid, iu.estuf, mun.estuf, mun.mundescricao
	                    ORDER BY
	                        escola, subacao, item
                	) foo
	                WHERE
	                    foo.quantidade > 0 ";
            $relatorio = $db->carregar($sqlRelatorio);
			
            if( $relatorio ) geraAnexoEscolas($relatorio, $v['dopid']);
	}
}