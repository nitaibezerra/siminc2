<?php

class EmendaImpositivo{
	private $ano, $ptrid;
	
	public function __construct( $ptrid, $ano ){
		$this->ptrid = $ptrid;
		$this->ano = $ano;		
	}
	
	public function carregarEmenda(){
		global $db;
		
		if( $_SESSION['exercicio'] > 2014 ) $filtroImped = " and ei.edeid = vede.edeid ";
		
		$sql = "SELECT
				'<center><img border=\"0\" title=\"Impedimentos\" src=\"../imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"abreEmpedimento('||vede.emdid||', \''||vede.edevalor||'\', '||vede.edeid||');\" alt=\"Ir\"/></center>' as acoes,
				vede.emecod,
				(CASE WHEN vede.emetipo = 'E' THEN 'Emenda' ELSE 'Complemento' END) as tipoemenda,
				case when vede.emerelator = 'S' then a.autnome||' - Relator Geral' else a.autnome end as autnome,
				vfun.fupfuncionalprogramatica,
				vfun.fupdsc,
				vede.gndcod||' - '||gn.gnddsc as gndcod1, 
	            vede.mapcod||' - '||map.mapdsc as modalidade,
	            vede.foncod||' - '||fon.fondsc as fonte,
				sum(vede.edevalor) - coalesce(ei.edivalor, 0) as valor
				--(select ptrid from emenda.ptemendadetalheentidade where edeid = vede.edeid limit 1) as pta
			FROM
				emenda.v_emendadetalheentidade vede
				inner join emenda.ptemendadetalheentidade pte on pte.edeid = vede.edeid
				inner join emenda.autor a on a.autid = vede.autid
				left join emenda.v_funcionalprogramatica vfun on vfun.acaid = vede.acaid and vfun.prgano >= '2012' and vfun.acastatus = 'A'
				inner join emenda.entidadebeneficiada enb on enb.enbid = vede.entid
				left join public.gnd gn on gn.gndcod = vede.gndcod and gn.gndstatus = 'A'
	            left join public.modalidadeaplicacao map on map.mapcod = vede.mapcod
	            left join public.fonterecurso fon on fon.foncod = vede.foncod and fon.fonstatus = 'A'
	            left join emenda.emendadetalheimpedimento ei on ei.emdid = vede.emdid  $filtroImped
			WHERE
				vede.edestatus = 'A'
				and vede.ededisponivelpta = 'S'
				and vede.emeano >= '{$this->ano}'
				and pte.ptrid = {$this->ptrid}
				and vede.emetipo = 'E'
				and vede.edeid in ( select edeid from emenda.emendapariniciativa )
			GROUP BY
	          vede.emecod, a.autnome, vfun.fupfuncionalprogramatica, vfun.fupdsc,
	          vede.emedescentraliza, vede.emdid, vede.edevalor, vede.foncod, vede.gndcod,
	          vede.mapcod, gn.gnddsc, map.mapdsc, fon.fondsc,vede.emetipo, ei.edivalor, vede.emerelator";
		
	     return $sql;   
	}
	
	public function vigenciaCadastroConvenio(){
		global $db;
		
		$dataAtual  = date('Y-m-d');
		$arData = explode('-', $dataAtual);
		$arrRetorno = array();
		
		#verifica se a data atual esta entre os dias 03 a 12 de fervereiro
		if( ($arData[2] >= 03 && $arData[2] <= 12) && $arData[1] == 2 ){
			$arrRetorno= array('identificar_emenda' 	=> 'S',
								'cadastrar_programa' 	=> 'N',
								'cadastrar_pta' 		=> 'N',
								'analise_tecnica' 		=> 'N',
								);
		} elseif( ($arData[2] >= 18 && $arData[2] <= 22) && $arData[1] == 2 ){
			$arrRetorno= array('identificar_emenda' 	=> 'N',
								'cadastrar_programa' 	=> 'S',
								'cadastrar_pta' 		=> 'N',
								'analise_tecnica' 		=> 'N',
								);
		} elseif( ($arData[2] >= 23 && $arData[1] == 2) || ($arData[2] >= 19 && $arData[1] == 3) ){
			$arrRetorno= array('identificar_emenda' 	=> 'N',
								'cadastrar_programa' 	=> 'N',
								'cadastrar_pta' 		=> 'S',
								'analise_tecnica' 		=> 'N',
								);
		} elseif( ($arData[2] >= 20 && $arData[1] == 3) || ($arData[2] >= 19 && $arData[1] == 4) ){
			$arrRetorno= array('identificar_emenda' 	=> 'N',
								'cadastrar_programa' 	=> 'N',
								'cadastrar_pta' 		=> 'N',
								'analise_tecnica' 		=> 'S',
								);
		}
			//ver($arrRetorno,d);
	}
}
?>