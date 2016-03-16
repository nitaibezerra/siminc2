<?php
class Emenda extends cls_banco{
	
	public function __construct(){
		parent::__construct();
	}
	
	public function __destruct(){
		if( isset($_SESSION['transacao'] ) ){
			pg_query( $this->link, 'rollback; ');
			unset( $_SESSION['transacao'] );
		}
	}
	
	public function pegaCnpj($ptrid = null){
		if($ptrid){
			return parent::pegaLinha("SELECT e.enbid, e.enbcnpj FROM emenda.planotrabalho pt
										inner join emenda.entidadebeneficiada e on pt.enbid = e.enbid WHERE pt.ptrid = ".$ptrid);
		}
	}
	 
	public function buscaEmendaDescentraliza( $ptrid, $enbid = '', $resid = '' ){
		
		$sql = "SELECT
				  	e.emedescentraliza
				FROM
				  	emenda.emenda e
				  	INNER JOIN emenda.emendadetalhe ed ON (e.emeid = ed.emeid) 
				  	INNER JOIN emenda.emendadetalheentidade ede ON (ed.emdid = ede.emdid)
				  	LEFT JOIN (SELECT ped.pedid, ped.edeid, ped.ptrid
				               FROM emenda.ptemendadetalheentidade ped
				    				inner join emenda.planotrabalho ptr
				                    	on (ptr.ptrid = ped.ptrid)
				               WHERE ptr.ptrstatus = 'A') ptede
				  		ON (ede.edeid = ptede.edeid)  
				WHERE
					(".(($ptrid) ? "ptede.ptrid = ".$ptrid." OR ptede.ptrid is null" : "ptede.ptrid is null").")
				  	AND ede.edestatus = 'A'
				  	".( $resid ? 'AND e.resid = '.$resid : '')."
				  	AND ede.enbid = ".(($ptrid) ? '(SELECT enbid FROM emenda.planotrabalho WHERE ptrid = '.$ptrid.')' : $enbid)."
				  	AND ede.ededisponivelpta = 'S'
				  	AND e.emeano >= ".$_SESSION['exercicio'] ;

		$federal = parent::pegaUm( $sql );
		$_SESSION['emenda']['federal'] = $federal ;
		
		return $federal;
	}
	
	public function enviaEmailAnalisePTA( $filtros = '', $post = '' ){
		$arPerfil = pegaPerfilArray( $_SESSION["usucpf"] );
		
		if(in_array(ADMINISTRADOR_INST, $arPerfil ) ){
			$resid 		 = recuperaResponsaveis( $_SESSION["usucpf"] );
			$filtroResid = count($resid) > 0 ? " AND (res.resid in (" . implode( ", ", $resid ) . ") OR res.resid is null)" : "";
		}
		
		if( !empty($post["iniid"]) || !empty($post['espid']) ){
			$filtroIniciativa = "INNER JOIN emenda.v_ptiniciativa pti ON ptr.ptrid = pti.ptrid ";
		}
		
		if( is_array( $post['espid'] ) ){
			$filtroEspef = "INNER JOIN emenda.iniciativaresponsavel ir on ir.resid = res.resid 
				INNER JOIN emenda.iniciativa ini on ini.iniid = ir.iniid
				INNER JOIN emenda.iniciativaespecificacao ine on ine.iniid = ini.iniid
				INNER JOIN emenda.especificacao esp on esp.espid = ine.espid
				inner join emenda.especificacao_programacaoexercicio epe on epe.espid = esp.espid
	            INNER JOIN emenda.ptiniciativaespecificacao ptie on ptie.iceid = ine.iceid
	            	and ptie.ptiid = pti.ptiid
					and ptie.ptestatus = 'A'";
		}
		
		if( !empty($post["uniid"]) ){
			$filtroUnidade = "INNER JOIN emenda.analise ana ON ptr.ptrid = ana.ptrid ";
		}
		
		$sql = "SELECT DISTINCT
					ptr.ptrcod,
					ptr.ptrid,
	                ent.enbid,
	                usu.usucpf,
	                usu.usuemail,
	                ent.enbemail
				FROM  emenda.planotrabalho ptr
				INNER JOIN emenda.entidadebeneficiada ent ON ptr.enbid = ent.enbid
				-- INNER JOIN  entidade.entidade ent ON ptr.entid = ent.entid 
				-- INNER JOIN  entidade.endereco ende ON ende.entid = ent.entid
				--	AND ende.endstatus = 'A' 
				INNER JOIN  territorios.municipio mun ON mun.muncod = ent.muncod 
				INNER JOIN emenda.responsavel res ON ptr.resid = res.resid
				INNER JOIN workflow.documento doc ON ptr.docid = doc.docid
				INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
	            inner join emenda.usuarioresponsabilidade ure on ure.enbid = ent.enbid
	            inner join seguranca.usuario usu on usu.usucpf = ure.usucpf
	            $filtroIniciativa
	 			$filtroEspef
	 			$filtroUnidade
				WHERE
				    ptr.ptrexercicio = ".$_SESSION['exercicio']."
				    --AND doc.esdid <> ".EM_ELABORACAO."
				    {$filtros}
			    	{$filtroResid}
			    	AND ptr.ptrid NOT IN (SELECT tt.ptridpai FROM emenda.planotrabalho tt WHERE tt.ptridpai = ptr.ptrid and tt.ptrstatus = 'A')
	
				ORDER BY 
					ent.enbid";
		    	
		return parent::carregar( $sql );
	}
}
?>