<?php
class NovasTurmas{
	private $muncod, $mesAtual, $anoExercicio, $anoCenso, $turid, $post;
	
	public function __construct( $arrPost ){
		
		$this->muncod 		= ($arrPost['muncod'] ? $arrPost['muncod'] : $_SESSION['proinfantil']['muncod']);
		$this->mesAtual 	= ($arrPost['ntmmmes'] ? $arrPost['ntmmmes'] : date('m'));
		$this->anoExercicio = ($arrPost['exercicio'] ? $arrPost['exercicio'] : (int)$_SESSION['exercicio']);
		$this->anoCenso 	= ($arrPost['anoCenso'] ? $arrPost['anoCenso'] : ((int)$_SESSION['exercicio'] - 1));
		$this->turid		= $arrPost['turid'];
		$this->post			= $arrPost;
	}
	
	public function carregaDadosMatriculaTurmaMes(){
		global $db;
		
		$sql = "SELECT 
					COALESCE(SUM(dc.ntcqtdalunocrecheparcialpublica),0) as ntcqtdalunocrecheparcialpublica,
					COALESCE(SUM(dc.ntcqtdalunocrecheintegralpublica),0) as ntcqtdalunocrecheintegralpublica,
					COALESCE(SUM(dc.ntcqtdalunopreescolaparcialpublica),0) as ntcqtdalunopreescolaparcialpublica,
					COALESCE(SUM(dc.ntcqtdalunopreescolaintegralpublica),0) as ntcqtdalunopreescolaintegralpublica,
					COALESCE(SUM(dc.ntcqtdalunocrecheparcialconveniada),0) as ntcqtdalunocrecheparcialconveniada,
					COALESCE(SUM(dc.ntcqtdalunocrecheintegralconveniada),0) as ntcqtdalunocrecheintegralconveniada,
					COALESCE(SUM(dc.ntcqtdalunopreescolaparcialconveniada),0) as ntcqtdalunopreescolaparcialconveniada,
					COALESCE(SUM(dc.ntcqtdalunopreescolaintegralconveniada),0) as ntcqtdalunopreescolaintegralconveniada,
					
					COALESCE(SUM(dc.ntcqtdturmacrechepublica),0) as ntcqtdturmacrechepublica,
					COALESCE(SUM(dc.ntcqtdturmapreescolapublica),0) as ntcqtdturmapreescolapublica,
					COALESCE(SUM(dc.ntcqtdturmaunificadapublica),0) as ntcqtdturmaunificadapublica,
					COALESCE(SUM(dc.ntcqtdturmacrecheconveniada),0) as ntcqtdturmacrecheconveniada,
					COALESCE(SUM(dc.ntcqtdturmapreescolaconveniada),0) as ntcqtdturmapreescolaconveniada,
					COALESCE(SUM(dc.ntcqtdturmaunificadaconveniada),0) as ntcqtdturmaunificadaconveniada,
					dm.ntmmid,
					dm.ntmmmes,
					dm.ntmmatendeedinf,
					COALESCE(dm.ntmmqtdmatriculacrecheparcialpublica, 0) as ntmmqtdmatriculacrecheparcialpublica,
					COALESCE(dm.ntmmqtdmatriculacrecheintegralpublica, 0) as ntmmqtdmatriculacrecheintegralpublica,
					COALESCE(dm.ntmmqtdmatriculapreescolaparcialpublica, 0) as ntmmqtdmatriculapreescolaparcialpublica,
					COALESCE(dm.ntmmqtdmatriculapreescolaintegralpublica, 0) as ntmmqtdmatriculapreescolaintegralpublica,
					COALESCE(dm.ntmmqtdmatriculacrecheparcialconveniada, 0) as ntmmqtdmatriculacrecheparcialconveniada,
					COALESCE(dm.ntmmqtdmatriculacrecheintegralconveniada, 0) as ntmmqtdmatriculacrecheintegralconveniada,
					COALESCE(dm.ntmmqtdmatriculapreescolaparcialconveniada, 0) as ntmmqtdmatriculapreescolaparcialconveniada,
					COALESCE(dm.ntmmqtdmatriculapreescolaintegralconveniada, 0) as ntmmqtdmatriculapreescolaintegralconveniada,
					COALESCE(dm.ntmmqtdturmacrechepublica, 0) as ntmmqtdturmacrechepublica,
					COALESCE(dm.ntmmqtdturmapreescolapublica, 0) as ntmmqtdturmapreescolapublica,
					COALESCE(dm.ntmmqtdturmaunificadapublica, 0) as ntmmqtdturmaunificadapublica,
					COALESCE(dm.ntmmqtdturmacrecheconveniada, 0) as ntmmqtdturmacrecheconveniada,
					COALESCE(dm.ntmmqtdturmapreescolaconveniada, 0) as ntmmqtdturmapreescolaconveniada,
					COALESCE(dm.ntmmqtdturmaunificadaconveniada, 0) as ntmmqtdturmaunificadaconveniada,
					dc.ntcanocenso					
				FROM proinfantil.novasturmasdadoscenso dc
					LEFT JOIN proinfantil.novasturmasdadosmunicipiospormes dm on dm.muncod = dc.muncod and dm.ntmmano = '".$this->anoExercicio."' and dm.ntmmmes = ".$this->mesAtual." and dm.ntmmstatus = 'A' 
				WHERE dc.muncod = '{$this->muncod}'
					AND dc.ntcanocenso = '".$this->anoCenso."'
					and dc.ntcstatus = 'A'
				GROUP BY
					dm.ntmmid,
					dm.ntmmqtdmatriculacrecheparcialpublica,
					dm.ntmmqtdmatriculacrecheintegralpublica,
					dm.ntmmqtdmatriculapreescolaparcialpublica,
					dm.ntmmqtdmatriculapreescolaintegralpublica,
					dm.ntmmqtdmatriculacrecheparcialconveniada,
					dm.ntmmqtdmatriculacrecheintegralconveniada,
					dm.ntmmqtdmatriculapreescolaparcialconveniada,
					dm.ntmmqtdmatriculapreescolaintegralconveniada,
					dc.ntcanocenso,
					dm.ntmmmes,
					dm.ntmmqtdturmacrechepublica,
					dm.ntmmqtdturmapreescolapublica,
					dm.ntmmqtdturmaunificadapublica,
					dm.ntmmqtdturmacrecheconveniada,
					dm.ntmmqtdturmapreescolaconveniada,
					dm.ntmmqtdturmaunificadaconveniada,
					dm.ntmmatendeedinf";
		//ver($sql,d);
		$arrMatTurma = $db->pegaLinha($sql);
		$arrMatTurma = $arrMatTurma ? $arrMatTurma : array();
		
		return $arrMatTurma;
	}
	
	public function carregaDadosMatriculaMes( $mes ){
		global $db;
		$maiorMes = $this->pegaMaiorMes();
		$maiorMes = $maiorMes ? $maiorMes : '0';
		
		$sql = "select
					case when totalTurma = 0 then ntcqtdalunocrecheparcialpublica else ntmmqtdmatriculacrecheparcialpublica1 end as ntcqtdalunocrecheparcialpublica,
					case when totalTurma = 0 then ntcqtdalunocrecheintegralpublica else ntmmqtdmatriculacrecheintegralpublica1 end as ntcqtdalunocrecheintegralpublica,
					case when totalTurma = 0 then ntcqtdalunopreescolaparcialpublica else ntmmqtdmatriculapreescolaparcialpublica1 end as ntcqtdalunopreescolaparcialpublica,	
					case when totalTurma = 0 then ntcqtdalunopreescolaintegralpublica else ntmmqtdmatriculapreescolaintegralpublica1 end as ntcqtdalunopreescolaintegralpublica,					
					case when totalTurma = 0 then ntcqtdalunocrecheparcialconveniada else ntmmqtdmatriculacrecheparcialconveniada1 end as ntcqtdalunocrecheparcialconveniada,
					case when totalTurma = 0 then ntcqtdalunocrecheintegralconveniada else ntmmqtdmatriculacrecheintegralconveniada1 end as ntcqtdalunocrecheintegralconveniada,
					case when totalTurma = 0 then ntcqtdalunopreescolaparcialconveniada else ntmmqtdmatriculapreescolaparcialconveniada1 end as ntcqtdalunopreescolaparcialconveniada,
					case when totalTurma = 0 then ntcqtdalunopreescolaintegralconveniada else ntmmqtdmatriculapreescolaintegralconveniada1 end as ntcqtdalunopreescolaintegralconveniada,
					ntmmqtdmatriculacrecheparcialpublica,
					ntmmqtdmatriculacrecheintegralpublica,
					ntmmqtdmatriculapreescolaparcialpublica,
					ntmmqtdmatriculapreescolaintegralpublica,					
					ntmmqtdmatriculacrecheparcialconveniada,					
					ntmmqtdmatriculacrecheintegralconveniada,
					ntmmqtdmatriculapreescolaparcialconveniada,
					ntmmqtdmatriculapreescolaintegralconveniada					
				from(
					SELECT 
						COALESCE(SUM(dc.ntcqtdalunocrecheparcialpublica),0) as ntcqtdalunocrecheparcialpublica,
						COALESCE(SUM(dc.ntcqtdalunocrecheintegralpublica),0) as ntcqtdalunocrecheintegralpublica,
						COALESCE(SUM(dc.ntcqtdalunopreescolaparcialpublica),0) as ntcqtdalunopreescolaparcialpublica,
						COALESCE(SUM(dc.ntcqtdalunopreescolaintegralpublica),0) as ntcqtdalunopreescolaintegralpublica,						
						COALESCE(SUM(dc.ntcqtdalunocrecheparcialconveniada),0) as ntcqtdalunocrecheparcialconveniada,
						COALESCE(SUM(dc.ntcqtdalunocrecheintegralconveniada),0) as ntcqtdalunocrecheintegralconveniada,
						COALESCE(SUM(dc.ntcqtdalunopreescolaparcialconveniada),0) as ntcqtdalunopreescolaparcialconveniada,
						COALESCE(SUM(dc.ntcqtdalunopreescolaintegralconveniada),0) as ntcqtdalunopreescolaintegralconveniada,
				
						(select count(ntmmid) from proinfantil.novasturmasdadosmunicipiospormes 
				        	where muncod = dc.muncod and ntmmano = '{$this->anoExercicio}' and ntmmmes = ".$maiorMes." and ntmmstatus = 'A') as totalTurma,
						
						COALESCE(dm.ntmmqtdmatriculacrecheparcialpublica, 0) as ntmmqtdmatriculacrecheparcialpublica,
						COALESCE(dm.ntmmqtdmatriculacrecheintegralpublica, 0) as ntmmqtdmatriculacrecheintegralpublica,
						COALESCE(dm.ntmmqtdmatriculapreescolaparcialpublica, 0) as ntmmqtdmatriculapreescolaparcialpublica,
						COALESCE(dm.ntmmqtdmatriculapreescolaintegralpublica, 0) as ntmmqtdmatriculapreescolaintegralpublica,
						
						COALESCE(dm.ntmmqtdmatriculacrecheparcialconveniada, 0) as ntmmqtdmatriculacrecheparcialconveniada,
						COALESCE(dm.ntmmqtdmatriculacrecheintegralconveniada, 0) as ntmmqtdmatriculacrecheintegralconveniada,
						COALESCE(dm.ntmmqtdmatriculapreescolaparcialconveniada, 0) as ntmmqtdmatriculapreescolaparcialconveniada,
						COALESCE(dm.ntmmqtdmatriculapreescolaintegralconveniada, 0) as ntmmqtdmatriculapreescolaintegralconveniada,
						
						COALESCE(dm1.ntmmqtdmatriculacrecheparcialpublica, 0) as ntmmqtdmatriculacrecheparcialpublica1,
						COALESCE(dm1.ntmmqtdmatriculacrecheintegralpublica, 0) as ntmmqtdmatriculacrecheintegralpublica1,
						COALESCE(dm1.ntmmqtdmatriculapreescolaparcialpublica, 0) as ntmmqtdmatriculapreescolaparcialpublica1,
						COALESCE(dm1.ntmmqtdmatriculapreescolaintegralpublica, 0) as ntmmqtdmatriculapreescolaintegralpublica1,
						
						COALESCE(dm1.ntmmqtdmatriculacrecheparcialconveniada, 0) as ntmmqtdmatriculacrecheparcialconveniada1,
						COALESCE(dm1.ntmmqtdmatriculacrecheintegralconveniada, 0) as ntmmqtdmatriculacrecheintegralconveniada1,
						COALESCE(dm1.ntmmqtdmatriculapreescolaparcialconveniada, 0) as ntmmqtdmatriculapreescolaparcialconveniada1,
						COALESCE(dm1.ntmmqtdmatriculapreescolaintegralconveniada, 0) as ntmmqtdmatriculapreescolaintegralconveniada1
					
				FROM proinfantil.novasturmasdadoscenso dc
					LEFT JOIN proinfantil.novasturmasdadosmunicipiospormes dm on dm.muncod = dc.muncod and dm.ntmmano = '".$this->anoExercicio."' and dm.ntmmmes = ".$mes." and dm.ntmmstatus = 'A'		
					LEFT JOIN proinfantil.novasturmasdadosmunicipiospormes dm1 on dm1.muncod = dc.muncod and dm1.ntmmano = '".$this->anoExercicio."' and dm1.ntmmmes = ".$maiorMes." and dm1.ntmmstatus = 'A'		
				WHERE dc.muncod = '{$this->muncod}'
					AND dc.ntcanocenso = '".$this->anoCenso."'
					and dc.ntcstatus = 'A'
				GROUP BY
					dm.ntmmqtdmatriculacrecheparcialpublica,
					dm.ntmmqtdmatriculacrecheintegralpublica,
					dm.ntmmqtdmatriculapreescolaparcialpublica,
					dm.ntmmqtdmatriculapreescolaintegralpublica,
					dm.ntmmqtdmatriculacrecheparcialconveniada,
					dm.ntmmqtdmatriculacrecheintegralconveniada,
					dm.ntmmqtdmatriculapreescolaparcialconveniada,
					dm.ntmmqtdmatriculapreescolaintegralconveniada,
					dm1.ntmmqtdmatriculacrecheparcialpublica,
					dm1.ntmmqtdmatriculacrecheintegralpublica,
					dm1.ntmmqtdmatriculapreescolaparcialpublica,
					dm1.ntmmqtdmatriculapreescolaintegralpublica,
					dm1.ntmmqtdmatriculacrecheparcialconveniada,
					dm1.ntmmqtdmatriculacrecheintegralconveniada,
					dm1.ntmmqtdmatriculapreescolaparcialconveniada,
					dm1.ntmmqtdmatriculapreescolaintegralconveniada,
					dc.muncod
			) as foo";
		
		$arrMatTurma = $db->pegaLinha($sql);
		$arrMatTurma = $arrMatTurma ? $arrMatTurma : array();
		
		return $arrMatTurma;
	}
	
	public function carregaDadosTurmaMaiorPorMes(){
		global $db;
		
		$maiorMes = $this->pegaMaiorMes();
		$maiorMes = $maiorMes ? $maiorMes : '0';
		
		$sql = "select 
					case when max(totalTurma) = 0 then max(ntcqtdturmacrechepublica) else max(ntmmqtdturmacrechepublica) end as ntmmqtdturmacrechepublica,
					case when max(totalTurma) = 0 then max(ntcqtdturmapreescolapublica) else max(ntmmqtdturmapreescolapublica) end as ntmmqtdturmapreescolapublica,
					case when max(totalTurma) = 0 then max(ntcqtdturmaunificadapublica) else max(ntmmqtdturmaunificadapublica) end as ntmmqtdturmaunificadapublica,
					
					case when max(totalTurma) = 0 then max(ntcqtdturmacrecheconveniada) else max(ntmmqtdturmacrecheconveniada) end as ntmmqtdturmacrecheconveniada,
					case when max(totalTurma) = 0 then max(ntcqtdturmapreescolaconveniada) else max(ntmmqtdturmapreescolaconveniada) end as ntmmqtdturmapreescolaconveniada,
					case when max(totalTurma) = 0 then max(ntcqtdturmaunificadaconveniada) else max(ntmmqtdturmaunificadaconveniada) end as ntmmqtdturmaunificadaconveniada,
					max(ntmmmes) as ntmmmes
				from(
				    SELECT					
				        COALESCE(SUM(dc.ntcqtdturmacrechepublica),0) as ntcqtdturmacrechepublica,
				        COALESCE(SUM(dc.ntcqtdturmapreescolapublica),0) as ntcqtdturmapreescolapublica,
				        COALESCE(SUM(dc.ntcqtdturmaunificadapublica),0) as ntcqtdturmaunificadapublica,
				        COALESCE(SUM(dc.ntcqtdturmacrecheconveniada),0) as ntcqtdturmacrecheconveniada,
				        COALESCE(SUM(dc.ntcqtdturmapreescolaconveniada),0) as ntcqtdturmapreescolaconveniada,
				        COALESCE(SUM(dc.ntcqtdturmaunificadaconveniada),0) as ntcqtdturmaunificadaconveniada,
				        dm.ntmmid,
				        dm.ntmmmes,
				        (select count(ntmmid) from proinfantil.novasturmasdadosmunicipiospormes 
				        	where muncod = dc.muncod and ntmmano = '{$this->anoExercicio}' and ntmmmes = ".$maiorMes." and ntmmstatus = 'A') as totalTurma,
				        COALESCE(dm.ntmmqtdturmacrechepublica, 0) as ntmmqtdturmacrechepublica,
				        COALESCE(dm.ntmmqtdturmapreescolapublica, 0) as ntmmqtdturmapreescolapublica,
				        COALESCE(dm.ntmmqtdturmaunificadapublica, 0) as ntmmqtdturmaunificadapublica,
				        COALESCE(dm.ntmmqtdturmacrecheconveniada, 0) as ntmmqtdturmacrecheconveniada,
				        COALESCE(dm.ntmmqtdturmapreescolaconveniada, 0) as ntmmqtdturmapreescolaconveniada,
				        COALESCE(dm.ntmmqtdturmaunificadaconveniada, 0) as ntmmqtdturmaunificadaconveniada,
				        dc.ntcanocenso					
				    FROM proinfantil.novasturmasdadoscenso dc
				    	LEFT JOIN proinfantil.novasturmasdadosmunicipiospormes dm on dm.muncod = dc.muncod and dm.ntmmano = '{$this->anoExercicio}' and dm.ntmmmes = ".$maiorMes." and dm.ntmmstatus = 'A'			
				    WHERE dc.muncod = '{$this->muncod}'
				    	AND dc.ntcanocenso = '{$this->anoCenso}'
				    	and dc.ntcstatus = 'A'
				    GROUP BY
				        dm.ntmmid,
				        dc.ntcanocenso,
				        dm.ntmmmes,
				        dm.ntmmqtdturmacrechepublica,
				        dm.ntmmqtdturmapreescolapublica,
				        dm.ntmmqtdturmaunificadapublica,
				        dm.ntmmqtdturmacrecheconveniada,
				        dm.ntmmqtdturmapreescolaconveniada,
				        dm.ntmmqtdturmaunificadaconveniada,
				        dc.muncod
				) as foo";
		
		$arrMatTurma = $db->pegaLinha($sql);
		$arrMatTurma = $arrMatTurma ? $arrMatTurma : array();
		
		return $arrMatTurma;
	}
	
	public function carregaDadosMatriculaMaiorPorMes(){
		global $db;
		
		$maiorMes = $this->pegaMaiorMes();
		$maiorMes = $maiorMes ? $maiorMes : '0';
		
		$sql = "select 
					--creche
					case when max(totalMatricula) = 0 then max(ntcqtdalunocrecheintegralpublica) else max(ntmmqtdmatriculacrecheintegralpublica) end as ntmmqtdmatriculacrecheintegralpublica,
					case when max(totalMatricula) = 0 then max(ntcqtdalunocrecheparcialpublica) else max(ntmmqtdmatriculacrecheparcialpublica) end as ntmmqtdmatriculacrecheparcialpublica,
					case when max(totalMatricula) = 0 then max(ntcqtdalunocrecheintegralconveniada) else max(ntmmqtdmatriculacrecheintegralconveniada) end as ntmmqtdmatriculacrecheintegralconveniada,
					case when max(totalMatricula) = 0 then max(ntcqtdalunocrecheparcialconveniada) else max(ntmmqtdmatriculacrecheparcialconveniada) end as ntmmqtdmatriculacrecheparcialconveniada,
					--preescola
					case when max(totalMatricula) = 0 then max(ntcqtdalunopreescolaintegralpublica) else max(ntmmqtdmatriculapreescolaintegralpublica) end as ntmmqtdmatriculapreescolaintegralpublica,
					case when max(totalMatricula) = 0 then max(ntcqtdalunopreescolaparcialpublica) else max(ntmmqtdmatriculapreescolaparcialpublica) end as ntmmqtdmatriculapreescolaparcialpublica,
					case when max(totalMatricula) = 0 then max(ntcqtdalunopreescolaintegralconveniada) else max(ntmmqtdmatriculapreescolaintegralconveniada) end as ntmmqtdmatriculapreescolaintegralconveniada,
					case when max(totalMatricula) = 0 then max(ntcqtdalunopreescolaparcialconveniada) else max(ntmmqtdmatriculapreescolaparcialconveniada) end as ntmmqtdmatriculapreescolaparcialconveniada,
					max(ntmmmes) as ntmmmes
				from(
					SELECT
						--creche censo
						COALESCE(SUM(dc.ntcqtdalunocrecheintegralpublica),0) as ntcqtdalunocrecheintegralpublica,
						COALESCE(SUM(dc.ntcqtdalunocrecheparcialpublica),0) as ntcqtdalunocrecheparcialpublica,
						COALESCE(SUM(dc.ntcqtdalunocrecheintegralconveniada),0) as ntcqtdalunocrecheintegralconveniada,
						COALESCE(SUM(dc.ntcqtdalunocrecheparcialconveniada),0) as ntcqtdalunocrecheparcialconveniada,
						--preescola censo
						COALESCE(SUM(dc.ntcqtdalunopreescolaintegralpublica),0) as ntcqtdalunopreescolaintegralpublica,
						COALESCE(SUM(dc.ntcqtdalunopreescolaparcialpublica),0) as ntcqtdalunopreescolaparcialpublica,
						COALESCE(SUM(dc.ntcqtdalunopreescolaintegralconveniada),0) as ntcqtdalunopreescolaintegralconveniada,
						COALESCE(SUM(dc.ntcqtdalunopreescolaparcialconveniada),0) as ntcqtdalunopreescolaparcialconveniada,
						dm.ntmmid,
						dm.ntmmmes,
						(select count(ntmmid) from proinfantil.novasturmasdadosmunicipiospormes 
				        	where muncod = dc.muncod and ntmmano = '{$this->anoExercicio}' and ntmmmes = ".$maiorMes." and ntmmstatus = 'A') as totalMatricula,
						--creche matricula
						COALESCE(dm.ntmmqtdmatriculacrecheintegralpublica, 0) as ntmmqtdmatriculacrecheintegralpublica,
						COALESCE(dm.ntmmqtdmatriculacrecheparcialpublica, 0) as ntmmqtdmatriculacrecheparcialpublica,
						COALESCE(dm.ntmmqtdmatriculacrecheintegralconveniada, 0) as ntmmqtdmatriculacrecheintegralconveniada,
						COALESCE(dm.ntmmqtdmatriculacrecheparcialconveniada, 0) as ntmmqtdmatriculacrecheparcialconveniada,
						--preescola matricula
						COALESCE(dm.ntmmqtdmatriculapreescolaintegralpublica, 0) as ntmmqtdmatriculapreescolaintegralpublica,
						COALESCE(dm.ntmmqtdmatriculapreescolaparcialpublica, 0) as ntmmqtdmatriculapreescolaparcialpublica,
						COALESCE(dm.ntmmqtdmatriculapreescolaintegralconveniada, 0) as ntmmqtdmatriculapreescolaintegralconveniada,
						COALESCE(dm.ntmmqtdmatriculapreescolaparcialconveniada, 0) as ntmmqtdmatriculapreescolaparcialconveniada,
						dc.ntcanocenso					
					FROM proinfantil.novasturmasdadoscenso dc
						LEFT JOIN proinfantil.novasturmasdadosmunicipiospormes dm on dm.muncod = dc.muncod and dm.ntmmano = '{$this->anoExercicio}' and dm.ntmmmes = ".$maiorMes." and dm.ntmmstatus = 'A'		
				    WHERE dc.muncod = '{$this->muncod}'
				    	AND dc.ntcanocenso = '{$this->anoCenso}'
				    	and dc.ntcstatus = 'A'
					GROUP BY
						dm.ntmmid,
						dc.ntcanocenso,
						dm.ntmmmes,
						dm.ntmmqtdmatriculacrecheintegralpublica,
						dm.ntmmqtdmatriculacrecheparcialpublica,
						dm.ntmmqtdmatriculacrecheintegralconveniada,
						dm.ntmmqtdmatriculacrecheparcialconveniada,
						
						dm.ntmmqtdmatriculapreescolaintegralpublica,
						dm.ntmmqtdmatriculapreescolaparcialpublica,
						dm.ntmmqtdmatriculapreescolaintegralconveniada,
						dm.ntmmqtdmatriculapreescolaparcialconveniada,
						dc.muncod
				) as foo";
		
		$arrMatTurma = $db->pegaLinha($sql);
		$arrMatTurma = $arrMatTurma ? $arrMatTurma : array();
		
		return $arrMatTurma;
	}
	
	public function pegaMaiorMes(){
		global $db;
		
		$sql = "select
				    ntmmmes,
				    max((ntmmqtdmatriculacrecheintegralpublica + ntmmqtdmatriculacrecheparcialpublica + ntmmqtdmatriculacrecheintegralconveniada + ntmmqtdmatriculacrecheparcialconveniada +
				      ntmmqtdmatriculapreescolaintegralpublica + ntmmqtdmatriculapreescolaparcialpublica + ntmmqtdmatriculapreescolaintegralconveniada + ntmmqtdmatriculapreescolaparcialconveniada)) as total
				from(
				    SELECT DISTINCT
				        dm.ntmmmes,
				        --creche matricula
				        COALESCE(dm.ntmmqtdmatriculacrecheintegralpublica, 0) as ntmmqtdmatriculacrecheintegralpublica,
				        COALESCE(dm.ntmmqtdmatriculacrecheparcialpublica, 0) as ntmmqtdmatriculacrecheparcialpublica,
				        COALESCE(dm.ntmmqtdmatriculacrecheintegralconveniada, 0) as ntmmqtdmatriculacrecheintegralconveniada,
				        COALESCE(dm.ntmmqtdmatriculacrecheparcialconveniada, 0) as ntmmqtdmatriculacrecheparcialconveniada,
				        --preescola matricula
				        COALESCE(dm.ntmmqtdmatriculapreescolaintegralpublica, 0) as ntmmqtdmatriculapreescolaintegralpublica,
				        COALESCE(dm.ntmmqtdmatriculapreescolaparcialpublica, 0) as ntmmqtdmatriculapreescolaparcialpublica,
				        COALESCE(dm.ntmmqtdmatriculapreescolaintegralconveniada, 0) as ntmmqtdmatriculapreescolaintegralconveniada,
				        COALESCE(dm.ntmmqtdmatriculapreescolaparcialconveniada, 0) as ntmmqtdmatriculapreescolaparcialconveniada					
				    FROM proinfantil.novasturmasdadosmunicipiospormes dm 
				    WHERE dm.muncod = '{$this->muncod}'
				        AND dm.ntmmano = '{$this->anoExercicio}'
				        AND dm.ntmmmes < '{$this->mesAtual}'
				        and dm.ntmmstatus = 'A'
				) as foo
				group by ntmmmes
				order by ntmmmes desc limit 1";
		
		$arrDados = $db->pegaLinha($sql);
		$mes = $arrDados['ntmmmes'];
		return $mes;
	}
	
	public function recuperarDadosMatriculasTurmasMes( $mesAtual = '' ){
		global $db;
		
		if( $mesAtual ) $filtro = " and dm.ntmmmes = $mesAtual";
		
		$sql = "SELECT		
					dm.ntmmid,						
					-- Matriculas
					COALESCE(SUM(ntcqtdalunocrecheparcialpublica),0) as ntcqtdalunocrecheparcialpublica,
					COALESCE(SUM(ntcqtdalunocrecheintegralpublica),0) as ntcqtdalunocrecheintegralpublica,
					COALESCE(SUM(ntcqtdalunopreescolaparcialpublica),0) as ntcqtdalunopreescolaparcialpublica,
					COALESCE(SUM(ntcqtdalunopreescolaintegralpublica),0) as ntcqtdalunopreescolaintegralpublica,
					COALESCE(SUM(ntcqtdalunocrecheparcialconveniada),0) as ntcqtdalunocrecheparcialconveniada,
					COALESCE(SUM(ntcqtdalunocrecheintegralconveniada),0) as ntcqtdalunocrecheintegralconveniada,
					COALESCE(SUM(ntcqtdalunopreescolaparcialconveniada),0) as ntcqtdalunopreescolaparcialconveniada,
					COALESCE(SUM(ntcqtdalunopreescolaintegralconveniada),0) as ntcqtdalunopreescolaintegralconveniada,
					COALESCE(ntmmqtdmatriculacrecheparcialpublica,0) as ntmmqtdmatriculacrecheparcialpublica,
					COALESCE(ntmmqtdmatriculacrecheintegralpublica,0) as ntmmqtdmatriculacrecheintegralpublica,
					COALESCE(ntmmqtdmatriculapreescolaparcialpublica,0) as ntmmqtdmatriculapreescolaparcialpublica,
					COALESCE(ntmmqtdmatriculapreescolaintegralpublica,0) as ntmmqtdmatriculapreescolaintegralpublica,
					COALESCE(ntmmqtdmatriculacrecheparcialconveniada,0) as ntmmqtdmatriculacrecheparcialconveniada,
					COALESCE(ntmmqtdmatriculacrecheintegralconveniada,0) as ntmmqtdmatriculacrecheintegralconveniada,
					COALESCE(ntmmqtdmatriculapreescolaparcialconveniada,0) as ntmmqtdmatriculapreescolaparcialconveniada,
					COALESCE(ntmmqtdmatriculapreescolaintegralconveniada,0) as ntmmqtdmatriculapreescolaintegralconveniada,
						
					-- Turmas
					COALESCE(SUM(ntcqtdturmacrechepublica),0) as ntcqtdturmacrechepublica,
					COALESCE(SUM(ntcqtdturmapreescolapublica),0) as ntcqtdturmapreescolapublica,
					COALESCE(SUM(ntcqtdturmaunificadapublica),0) as ntcqtdturmaunificadapublica,
					COALESCE(SUM(ntcqtdturmacrecheconveniada),0) as ntcqtdturmacrecheconveniada,
					COALESCE(SUM(ntcqtdturmapreescolaconveniada),0) as ntcqtdturmapreescolaconveniada,
					COALESCE(SUM(ntcqtdturmaunificadaconveniada),0) as ntcqtdturmaunificadaconveniada,
					COALESCE(ntmmqtdturmacrechepublica,0) as ntmmqtdturmacrechepublica,
					COALESCE(ntmmqtdturmapreescolapublica,0) as ntmmqtdturmapreescolapublica,
					COALESCE(ntmmqtdturmaunificadapublica,0) as ntmmqtdturmaunificadapublica,
					COALESCE(ntmmqtdturmacrecheconveniada,0) as ntmmqtdturmacrecheconveniada,
					COALESCE(ntmmqtdturmapreescolaconveniada,0) as ntmmqtdturmapreescolaconveniada,
					COALESCE(ntmmqtdturmaunificadaconveniada,0) as ntmmqtdturmaunificadaconveniada
						
				FROM proinfantil.novasturmasdadoscenso dc
				LEFT JOIN proinfantil.novasturmasdadosmunicipiospormes dm on dm.muncod = dc.muncod and dm.ntmmano = '".$this->anoExercicio."' and dm.ntmmstatus = 'A'
				WHERE dc.muncod = '{$this->muncod}'
				AND dc.ntcanocenso = '".$this->anoCenso."'
				and dc.ntcstatus = 'A' $filtro
				GROUP BY
					dm.ntmmid,
						
					-- Matriculas
					ntmmqtdmatriculacrecheparcialpublica,
					ntmmqtdmatriculacrecheintegralpublica,
					ntmmqtdmatriculapreescolaparcialpublica,
					ntmmqtdmatriculapreescolaintegralpublica,
					ntmmqtdmatriculacrecheparcialconveniada,
					ntmmqtdmatriculacrecheintegralconveniada,
					ntmmqtdmatriculapreescolaparcialconveniada,
					ntmmqtdmatriculapreescolaintegralconveniada,
						
					-- Turmas
					ntmmqtdturmacrechepublica,
					ntmmqtdturmapreescolapublica,
					ntmmqtdturmaunificadapublica,
					ntmmqtdturmacrecheconveniada,
					ntmmqtdturmapreescolaconveniada,
					ntmmqtdturmaunificadaconveniada";
		
		$arrDados = $db->pegaLinha($sql);
		$arrDados = $arrDados ? $arrDados : array();
		
		return $arrDados;
	}
	
	public function carregaDadosMatriculaTurmaMunicipio(){
		global $db;
		
		$sql = "SELECT ntmmid, muncod, ntmmstatus, ntmmano, ntmmmes, ntmmqtdmatriculacrecheparcialpublica, ntmmqtdmatriculacrecheintegralpublica,
  					ntmmqtdmatriculapreescolaparcialpublica, ntmmqtdmatriculapreescolaintegralpublica, ntmmqtdmatriculacrecheparcialconveniada,
  					ntmmqtdmatriculacrecheintegralconveniada, ntmmqtdmatriculapreescolaparcialconveniada, ntmmqtdmatriculapreescolaintegralconveniada,
  					ntmmqtdturmacrechepublica, ntmmqtdturmapreescolapublica, ntmmqtdturmaunificadapublica, ntmmqtdturmacrecheconveniada,
  					ntmmqtdturmapreescolaconveniada, ntmmqtdturmaunificadaconveniada
				FROM proinfantil.novasturmasdadosmunicipiospormes WHERE ntmmano = '{$this->anoExercicio}' and ntmmstatus = 'A' and muncod = '{$this->muncod}' and ntmmstatus = 'A'
				order by ntmmmes desc";
		
		$arrMatricula = $db->carregar($sql);
		$arrMatricula = $arrMatricula ? $arrMatricula : array();
		
		return $arrMatricula;
	}
	
	public function quantidadeTurmasCadastrar( $ntmmid, $boCadastraTurma, $arrCadTurma ){
		global $db;
		
		if( is_array($arrCadTurma) ){
			
			foreach ($arrCadTurma as $turma => $arTurma) {
									
				if( $arTurma['boCadastra'] == 'S' && (int)$arTurma['qtdTurma'] > 0 ){
					for( $i=0; $i<$arTurma['qtdTurma'] ; $i++ ){
						$sql = "INSERT INTO proinfantil.turma(muncod, docid, turdsc, usucpfinclusao, turstatus, turcadeducacenso, turestabelecimentoautorizado, ttuid, 
									turtipoestabelecimento, tirid, tatid, turdtinclusao, turmes, turano) 
								VALUES ('{$this->muncod}', null, '{$arTurma['descricao']}', '{$_SESSION['usucpf']}', 'A', false, false, {$arTurma['tipoTurma']}, 
									null, {$arTurma['tipoRede']}, null, now(), {$this->mesAtual}, {$this->anoExercicio}) returning turid ";
						
						$turid = $db->pegaUm( $sql );
						$this->turid = $turid;
						
						$docid = $this->criaDocumentoNovasTurmas();
					}
				}
			}				
			$db->commit();
		}
		
	}
		
	public function salvarMatriculaTurmaMunicipio( $arrPost, $boCadastraTurma, $boTemDireito, $arrCadTurma ){
		global $db;
		
		$muncod 										= $this->muncod;
		$ntmmano 										= $this->anoExercicio;
		$ntmmid											= $arrPost['ntmmid'];
		$ntmmmes 										= $arrPost['ntmmmes'] ? $arrPost['ntmmmes'] : $this->mesAtual;
		$ntmmatendeedinf								= $arrPost['ntmmatendeedinf'];
		$ntmmqtdmatriculacrecheparcialpublica			= $arrPost['ntmmqtdmatriculacrecheparcialpublica'] 			? $arrPost['ntmmqtdmatriculacrecheparcialpublica'] : '0';
		$ntmmqtdmatriculacrecheintegralpublica			= $arrPost['ntmmqtdmatriculacrecheintegralpublica'] 		? $arrPost['ntmmqtdmatriculacrecheintegralpublica'] : '0';
		$ntmmqtdmatriculapreescolaparcialpublica		= $arrPost['ntmmqtdmatriculapreescolaparcialpublica'] 		? $arrPost['ntmmqtdmatriculapreescolaparcialpublica'] : '0';
		$ntmmqtdmatriculapreescolaintegralpublica		= $arrPost['ntmmqtdmatriculapreescolaintegralpublica'] 		? $arrPost['ntmmqtdmatriculapreescolaintegralpublica'] : '0';
		$ntmmqtdmatriculacrecheparcialconveniada		= $arrPost['ntmmqtdmatriculacrecheparcialconveniada'] 		? $arrPost['ntmmqtdmatriculacrecheparcialconveniada'] : '0';
		$ntmmqtdmatriculacrecheintegralconveniada		= $arrPost['ntmmqtdmatriculacrecheintegralconveniada'] 		? $arrPost['ntmmqtdmatriculacrecheintegralconveniada'] : '0';
		$ntmmqtdmatriculapreescolaparcialconveniada		= $arrPost['ntmmqtdmatriculapreescolaparcialconveniada'] 	? $arrPost['ntmmqtdmatriculapreescolaparcialconveniada'] : '0';
		$ntmmqtdmatriculapreescolaintegralconveniada	= $arrPost['ntmmqtdmatriculapreescolaintegralconveniada']	? $arrPost['ntmmqtdmatriculapreescolaintegralconveniada'] : '0';
		
		$ntmmqtdturmacrechepublica						= $arrPost['ntmmqtdturmacrechepublica'] 		? $arrPost['ntmmqtdturmacrechepublica'] : '0';
		$ntmmqtdturmapreescolapublica					= $arrPost['ntmmqtdturmapreescolapublica'] 		? $arrPost['ntmmqtdturmapreescolapublica'] : '0';
		$ntmmqtdturmaunificadapublica					= $arrPost['ntmmqtdturmaunificadapublica'] 		? $arrPost['ntmmqtdturmaunificadapublica'] : '0';
		$ntmmqtdturmacrecheconveniada					= $arrPost['ntmmqtdturmacrecheconveniada']		? $arrPost['ntmmqtdturmacrecheconveniada'] : '0';
		$ntmmqtdturmapreescolaconveniada				= $arrPost['ntmmqtdturmapreescolaconveniada'] 	? $arrPost['ntmmqtdturmapreescolaconveniada'] : '0';
		$ntmmqtdturmaunificadaconveniada				= $arrPost['ntmmqtdturmaunificadaconveniada'] 	? $arrPost['ntmmqtdturmaunificadaconveniada'] : '0';
		
		if($ntmmid){
			$sql = "update proinfantil.novasturmasdadosmunicipiospormes set
						ntmmqtdmatriculacrecheparcialpublica 			= {$ntmmqtdmatriculacrecheparcialpublica},
						ntmmqtdmatriculacrecheintegralpublica 			= {$ntmmqtdmatriculacrecheintegralpublica},
						ntmmqtdmatriculapreescolaparcialpublica 		= {$ntmmqtdmatriculapreescolaparcialpublica},
						ntmmqtdmatriculapreescolaintegralpublica 		= {$ntmmqtdmatriculapreescolaintegralpublica},
						ntmmqtdmatriculacrecheparcialconveniada 		= {$ntmmqtdmatriculacrecheparcialconveniada},
						ntmmqtdmatriculacrecheintegralconveniada 		= {$ntmmqtdmatriculacrecheintegralconveniada},
						ntmmqtdmatriculapreescolaparcialconveniada 		= {$ntmmqtdmatriculapreescolaparcialconveniada},
						ntmmqtdmatriculapreescolaintegralconveniada 	= {$ntmmqtdmatriculapreescolaintegralconveniada},
						ntmmqtdturmacrechepublica 						= {$ntmmqtdturmacrechepublica},
						ntmmqtdturmapreescolapublica 					= {$ntmmqtdturmapreescolapublica},
						ntmmqtdturmaunificadapublica 					= {$ntmmqtdturmaunificadapublica},
						ntmmqtdturmacrecheconveniada 					= {$ntmmqtdturmacrecheconveniada},
						ntmmqtdturmapreescolaconveniada 				= {$ntmmqtdturmapreescolaconveniada},
						ntmmqtdturmaunificadaconveniada 				= {$ntmmqtdturmaunificadaconveniada},
						ntmmatendeedinf				 					= '{$ntmmatendeedinf}'
					where ntmmid = {$ntmmid} and muncod = '{$_SESSION['proinfantil']['muncod']}' and ntmmmes = $ntmmmes";
				$db->executar($sql);
		} else {
			
			$sql = "insert into proinfantil.novasturmasdadosmunicipiospormes
						(muncod,ntmmano,ntmmmes,ntmmqtdmatriculacrecheparcialpublica,ntmmqtdmatriculacrecheintegralpublica,
						ntmmqtdmatriculapreescolaparcialpublica,ntmmqtdmatriculapreescolaintegralpublica,ntmmqtdmatriculacrecheparcialconveniada,
						ntmmqtdmatriculacrecheintegralconveniada,ntmmqtdmatriculapreescolaparcialconveniada,ntmmqtdmatriculapreescolaintegralconveniada,
						ntmmqtdturmacrechepublica,ntmmqtdturmapreescolapublica,ntmmqtdturmaunificadapublica, ntmmqtdturmacrecheconveniada,
						ntmmqtdturmapreescolaconveniada,ntmmqtdturmaunificadaconveniada, ntmmatendeedinf)
					values
						('{$muncod}', '{$ntmmano}', '{$ntmmmes}', {$ntmmqtdmatriculacrecheparcialpublica}, {$ntmmqtdmatriculacrecheintegralpublica},
						{$ntmmqtdmatriculapreescolaparcialpublica}, {$ntmmqtdmatriculapreescolaintegralpublica}, {$ntmmqtdmatriculacrecheparcialconveniada},
						{$ntmmqtdmatriculacrecheintegralconveniada}, {$ntmmqtdmatriculapreescolaparcialconveniada}, {$ntmmqtdmatriculapreescolaintegralconveniada},
						{$ntmmqtdturmacrechepublica}, {$ntmmqtdturmapreescolapublica}, {$ntmmqtdturmaunificadapublica}, {$ntmmqtdturmacrecheconveniada}, 
						{$ntmmqtdturmapreescolaconveniada}, {$ntmmqtdturmaunificadaconveniada}, '{$ntmmatendeedinf}') returning ntmmid;";
						
			$ntmmid =$db->pegaUm($sql);
		}
		
		$sql = "select t.turid from proinfantil.turma t 
				where t.muncod = '{$this->muncod}' 
					and t.turmes = '{$this->mesAtual}' 
					and t.turano = '{$this->anoExercicio}'
					and t.turid not in (select turid from proinfantil.novasturmasworkflowturma where muncod = '{$this->muncod}' and ntwdataenvio is not null)";
		
		$arrTurid = $db->carregarColuna($sql);
		$arrTurid = $arrTurid ? $arrTurid : array();
		
		if( $arrTurid[0] ){
			$sql = "DELETE FROM proinfantil.novasturmasdiligencia WHERE turid in (".implode(', ', $arrTurid).") and (diltexto = 'null' or diltexto is null)";
			$db->executar($sql);
			$sql = "delete from proinfantil.novasturmasalunoatendido where turid in (".implode(', ', $arrTurid).")";
			$db->executar($sql);
			$sql = "delete from proinfantil.fotos where turid in (".implode(', ', $arrTurid).")";
			$db->executar($sql);
			$sql = "delete from proinfantil.novasturmasworkflowturma where turid in (".implode(', ', $arrTurid).")";
			$db->executar($sql);
			$sql = "delete from proinfantil.turma WHERE turid in (".implode(', ', $arrTurid).")";
			$db->executar($sql);
		}
		
		if( $boTemDireito ){
			$this->quantidadeTurmasCadastrar( $ntmmid, $boCadastraTurma, $arrCadTurma );
		}
		
		return $db->commit();
	}

	public function carregarTurma(){
		global $db;
		
		$sql = "SELECT
					tur.turid, tur.entcodent, tur.turdsc, tur.turdtinicio, ent.entcodent || ' - ' || ent.entnome as codinep, 
					muncod, docid, turdtinclusao, usucpfinclusao, turstatus, turcadeducacenso, turestabelecimentoautorizado,
  					turnomeescola, arqid, ttuid, turenderecoescola, turtipoestabelecimento, turlatitude, turlongitude, turcepescola, turenderecocodinep, turnumeroato,
  					turdatapublicacaoato, turtipoconselho, tirid, tatid, turmes, turano, turenderecoturma, turnometurma, turcepturma, turlatitudeturma, turlongitudeturma
  				FROM proinfantil.turma tur
  					left join entidade.entidade ent on trim(ent.entcodent) = trim(tur.entcodent)
  				WHERE turid = ".$this->turid;
		
		$arrDados = $db->pegaLinha($sql);
		$arrDados = ($arrDados ? $arrDados : array());
		$this->mesAtual = $arrDados['turmes'];
		return $arrDados;
	}
	
	public function recuperarDadosMatriculasCadastradas(){	
		global $db;
		
		$sql = "SELECT t.turtipoestabelecimento,  t.ttuid, t.tirid, maa.titid, COALESCE(SUM(maa.alaquantidade),0) as qtdalunos
				FROM proinfantil.turma t
				INNER JOIN proinfantil.novasturmasalunoatendido maa ON maa.turid = t.turid
				WHERE t.muncod = '{$this->muncod}'
				AND t.turid NOT IN ({$this->turid})
				AND t.turstatus = 'A'
				-- AND t.turid = 51
				GROUP BY t.turtipoestabelecimento,  t.ttuid, t.tirid, maa.titid";
	
		$arrDados = $db->carregar($sql);
		$arrDados = ($arrDados ? $arrDados : array());
		
		return $arrDados;
	}
	
	public function salvarFotosSalaNovasTurmas(){
		global $db;
		
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		
		$arArquivosPermitidos  = array('image/png', 'image/jpeg', 'image/gif', 'image/bmp');	
		
		if(!in_array($_FILES['arquivo']['type'], $arArquivosPermitidos)){
			echo "<script>
					alert('O tipo do arquivo não é permitido!');
					document.location.href = 'proinfantil.php?modulo=novasturmas/popupCadastrarTurmas&acao=A&turid={$this->turid}';
				  </script>";
			die;
		}
		
		if( $_FILES['arquivo']['tmp_name'] ){
			$arrCampos = array(
							"salid" => $this->post['salid'],
							"turid" => $this->turid,
							"usucpf" => "'{$_SESSION['usucpf']}'",
							"fotstatus" => "'A'",
							"fotdatainclusao" => "now()"
						      );
			$file = new FilesSimec("fotos", $arrCampos, "proinfantil");
			$file->setUpload($this->post->arqdescricao, "arquivo");
			if($db->commit()){
				/*if($this->post['foto'] == 'turma'){
					echo "<script>document.location.href = 'proinfantil.php?modulo=novasturmas/popupFormTurma&acao=A&escolhaTurma={$escolhaTurma}';</script>";
				} else {*/
					echo "<script>
							alert('Operação Realizada com Sucesso!');
							document.location.href = 'proinfantil.php?modulo=novasturmas/popupCadastrarTurmas&acao=A&turid={$this->turid}';
						</script>";
				//}
			}
			die();
		} else {
			/*if($this->post['foto'] == 'turma'){
				echo "<script>document.location.href = 'proinfantil.php?modulo=novasturmas/popupFormTurma&acao=A&escolhaTurma={$escolhaTurma}';</script>";
			} else {*/
				echo "<script>
						document.location.href = 'proinfantil.php?modulo=novasturmas/popupCadastrarTurmas&acao=A&turid={$this->turid}';
					</script>";
			//}
		}
	}

	public function removerFotoSalaNovasTurmas(){
		global $db;
		
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		
		$file = new FilesSimec("fotos", array(),"proinfantil");
		$file->excluiArquivoFisico($this->post['arqid']);
		$sql = "delete from proinfantil.fotos where arqid = {$this->post['arqid']};";
		$sql.= "delete from public.arquivo where arqid = {$this->post['arqid']};";
		$db->executar($sql);
		return $db->commit();
	}
	
	public function criaDocumentoNovasTurmas() {	
		global $db;
		
		if(empty($this->muncod)) return false;
		
		$docid = $this->pegaDocidNovasTurmas();
		
		if( !$docid ){
					
			$tpdid = WF_TPDID_PROINFANTIL_NOVASTURMAS;
			
			$docdsc = "Cadastramento sistema de informação ao cidadão";
			
			/*
			 * cria documento WORKFLOW
			 */
			$docid = wf_cadastrarDocumento( $tpdid, $docdsc );		
			
			$sql = "INSERT INTO proinfantil.novasturmasworkflowturma(turid, docid, muncod) 
					VALUES ({$this->turid}, {$docid}, '{$this->muncod}')";
	
			$db->executar( $sql );		
			$db->commit();
			return $docid;
			
		}
		else {
			return $docid;
		}
	}
	
	public function pegaDocidNovasTurmas() {
		global $db;	
		if( $this->turid ) $filtroTur = " and turid = {$this->turid} ";
		
		$sql = "SELECT docid FROM proinfantil.novasturmasworkflowturma
				WHERE muncod  = '" . $this->muncod . "' $filtroTur /*and ntwmesenvio = {$this->mesAtual} and ntwanoenvio = {$this->anoExercicio}*/";
		
		return (integer) $db->pegaUm( $sql );
	}
	
	public function pegaEstadoAtualNovasTurmas( $docid ) {
		global $db; 
		
		if($docid) {
			$docid = (integer) $docid;
			 
			$sql = "select ed.esdid from workflow.documento d
						inner join workflow.estadodocumento ed on ed.esdid = d.esdid
					where
						d.docid = " . $docid;
			$estado = $db->pegaUm( $sql );
			 
			return $estado;
		} else {
			return false;
		}
	}
	
	/*public function criaAbaNovasTurmas( $tiposAba = 1 ){
		if( $tiposAba == 1){
			$abasPar = array( 0 => array( "descricao" => "Questionario", 	"link"	  => "proinfantil.php?modulo=novasturmas/questionarioAtendimento&acao=A&abas=questionario&muncod={$_REQUEST['muncod']}"),
							  1 => array( "descricao" => "Matricula", 		"link"	  => "proinfantil.php?modulo=novasturmas/questionarioAtendimento&acao=A&abas=questionarioMatricula&muncod={$_REQUEST['muncod']}"),
							  2 => array( "descricao" => "Turma", 			"link"	  => "proinfantil.php?modulo=novasturmas/questionarioAtendimento&acao=A&abas=questionarioTurmas&muncod={$_REQUEST['muncod']}")
							 );
		}
		return $abasPar;
	}*/
	
	public function salvarDadosTurma( $post ){
		global $db;
		$file = new FilesSimec();
		
		$codinep 						= explode("-",$post['codinep']);
		$muncod							= $_SESSION['proinfantil']['muncod'];
		$turdtinicio 					= $post['turdtinicio'] ? formata_data_sql($post['turdtinicio']) : 'null';
		$turdtinclusao 					= date('Y-m-d H:i:s');
		$usucpfinclusao 				= $_SESSION['usucpf'];
		$turcadeducacenso 				= $post['turcadeducacenso'] == 'S' ? 't' : 'f';
		$turestabelecimentoautorizado 	= $post['turestabelecimentoautorizado'] == 'S' ? 't' : 'f';
		
		$turnomeescola 					= $post['turnomeescola'] ? $post['turnomeescola'] : '';
		$turenderecoescola				= $post['turenderecoescola'];
		$turlatitude					= $post['turlatitude']; 
		$turlongitude					= $post['turlongitude'];
		$turcepescola					= str_replace(array('.', '-'), '', $post['turcepescola']);
		
		$turnometurma 					= $post['turnometurma'] ? $post['turnometurma'] : '';
		$turenderecoturma				= $post['turenderecoturma'];
		$turlatitudeturma				= trim($post['turlatitudeturma']); 
		$turlongitudeturma				= trim($post['turlongitudeturma']);
		$turcepturma					= str_replace(array('.', '-'), '', $post['turcepturma']);
		
		$entcodent 						= trim($codinep[0]);
		$ttuid							= $post['ttuid'] ? $post['ttuid'] : 'null';
		$turtipoestabelecimento 		= $post['turtipoestabelecimento'] ? $post['turtipoestabelecimento'] : 'null';
		$turenderecocodinep 			= $post['turenderecocodinep'] ? "'".$post['turenderecocodinep']."'" : 'null';
		$turemitidoconselho 			= $post['turemitidoconselho'] ? "'".$post['turemitidoconselho']."'" : 'null';
		$turtipoconselho				= $post['turtipoconselho']; 
		$turnumeroato 					= $post['turnumeroato'];
		$turdatapublicacaoato 			= $post['turdatapublicacaoato'] ? "'".formata_data_sql($post['turdatapublicacaoato'])."'" : 'null';
		$tirid 							= $post['tirid'] ? $post['tirid'] : 'null';
		$anaid 							= $post['anaid'] ? $post['anaid'] : 'null';
		$diltexto 						= $post['diltexto'] ? $post['diltexto'] : 'null';
		$esdid							= $post['esdid'];
		$docid							= $post['docid'];
		$turmes							= $post['turmes'];
		$updateArquivo = '';
		$insertCampoArquivo = '';
		$insertValueArquivo = '';
				
		if( $turenderecocodinep == "'t'" ){
			$turnometurma = '';
			$turcepturma = '';
			$turenderecoturma = '';
		}
		
		if( $turcadeducacenso == "t" ){
			$turnomeescola = '';
			$turcepescola = '';
			$turenderecoescola = '';
		}
				
		if(is_file($_FILES["arquivo"]["tmp_name"]) ){
			$boSalvouArq = $file->setUpload($_FILES["arquivo"]["name"], "arquivo", false);
			if($boSalvouArq){
				$arquivoSalvo = $file->getIdArquivo();
				$updateArquivo = ",arqid={$arquivoSalvo}";
				$insertCampoArquivo = ", arqid";
				$insertValueArquivo = ", {$arquivoSalvo}";
			}		
		}
		$arquivoSalvo = $arquivoSalvo ? $arquivoSalvo : 'null';
		
		//if($post['turid']){
		$sql = "UPDATE proinfantil.turma SET 
					entcodent = '{$entcodent}', 
					turdsc = '{$post['turdsc']}', 
					turdtinicio = '{$turdtinicio}',
					turcadeducacenso='{$turcadeducacenso}',
					turestabelecimentoautorizado='{$turestabelecimentoautorizado}',
					turnomeescola='{$turnomeescola}'
					{$updateArquivo}
					,ttuid={$ttuid}
					,turenderecoescola='{$turenderecoescola}'
					,turtipoestabelecimento={$turtipoestabelecimento}
					,turlatitude='{$turlatitude}'
					,turlongitude='{$turlongitude}'
					,turcepescola='{$turcepescola}'
					,turenderecocodinep={$turenderecocodinep}					
					,turnumeroato='{$turnumeroato}'
					,turdatapublicacaoato={$turdatapublicacaoato}
					,turtipoconselho='{$turtipoconselho}'
					,tirid={$tirid},
					turenderecoturma = '{$turenderecoturma}',
					turnometurma = '{$turnometurma}',
					turcepturma = '{$turcepturma}',
					turlatitudeturma = '{$turlatitudeturma}',
					turlongitudeturma = '{$turlongitudeturma}'
				WHERE 
					turid = {$post['turid']}";
		//ver($post, $sql,d);
		$db->executar($sql);
		$turid = $post['turid'];
		
		if($turid){
			if(!empty($post['ntaquantidade_creche'][0]) || !empty($post['ntaquantidade_creche'][1])){
				$sql = "DELETE FROM proinfantil.novasturmasalunoatendido WHERE turid = {$turid} and timid = 6;";
				foreach($post['ntaquantidade_creche'] as $key=>$value){
					$ntaquantidade_creche = $value && in_array($ttuid, array(1,3)) ? str_replace('.','',$value) : '0';
					$ntaquantidadeprofessor_creche = $post['ntaquantidadeprofessor_creche'][$key] && in_array($ttuid, array(1,3)) ? str_replace('.','',$post['ntaquantidadeprofessor_creche'][$key]) : '0';
					
					if($post['creche'][$key] == "S"){
						$sql .= "INSERT INTO proinfantil.novasturmasalunoatendido 
									(ntaquantidade, tatid, turid, timid, ntaquantidadeprofessor)
							 	VALUES
							 		({$ntaquantidade_creche}, '{$post['tatid'][$key]}', '{$turid}', '6', {$ntaquantidadeprofessor_creche});";
					}			
				}
				$db->executar($sql);			
			}
			
			if(!empty($post['ntaquantidade_pre'][0]) || !empty($post['ntaquantidade_pre'][1])){
				$sql = "DELETE FROM proinfantil.novasturmasalunoatendido WHERE turid = {$turid} AND timid = 7;";
				foreach($post['ntaquantidade_pre'] as $key=>$value){
					$ntaquantidade_pre = $value && in_array($ttuid, array(2,3)) ? str_replace('.','',$value) : '0';
					$ntaquantidadeprofessor_pre = $post['ntaquantidadeprofessor_pre'][$key] && in_array($ttuid, array(2,3)) ? str_replace('.','',$post['ntaquantidadeprofessor_pre'][$key]) : '0';
					
					if($post['pre'][$key] == "S"){
					$sql .= "INSERT INTO proinfantil.novasturmasalunoatendido 
								(ntaquantidade, tatid, turid, timid, ntaquantidadeprofessor)
							 VALUES
							 	({$ntaquantidade_pre}, '{$post['tatid'][$key]}', '{$turid}', '7', {$ntaquantidadeprofessor_pre});";
					}
				}
				$db->executar($sql);			
			}		
		}
		
		if( $post['diltexto'] ){
			$sql = "UPDATE proinfantil.novasturmasdiligencia SET dilstatus = 'I' WHERE turid = $turid and anaid = $anaid";
			$db->executar($sql);
			$sql = "INSERT INTO proinfantil.novasturmasdiligencia(anaid, turid, usucpf, dildata, diltexto, dilstatus) 
					VALUES ($anaid, $turid, '{$_SESSION['usucpf']}', now(), '{$post['diltexto']}', 'A')";
			$db->executar($sql);
		}
		
		$db->commit();
		$db->sucesso('novasturmas/popupCadastrarTurmas','&turid='.$post['turid'].'&muncod='.$muncod);
		exit();
	}
	
	public function carregaTurmaPorMunicipio($whereAba = false){
		
		global $db;
				
		$filtro = '';
		
		if( $this->turid ){
			$filtro .= " and t.turid = ".$this->turid;
		}
		if( $this->post['tipo'] ){
			if( $this->post['tipo'] != 'C' ) $filtro .= " and n.anatipo = '{$this->post['tipo']}'";
		}		
		if( $this->post['esdidfiltro'] ){
			$filtro .= " and esd.esdid = ".$this->post['esdidfiltro'];			
		}
		if( $this->post['anatipofiltro'] ){
			$filtro .= " and n.anatipo = '{$this->post['anatipofiltro']}'";
		}
		if( $this->post['lotid'] ){
			$filtro .= " and t.lotid = '{$this->post['lotid']}'";
		}
				
		$sql = "select 
					turid,
					docid,
				    turdsc,
				    tirdescricao,
				    tipoatendimento,
				    entcodent,
				    turdtinicio,
				    dtanalise,
				    tipoanalise,
				    anaid,
				    anatipo,
					lotdsc,
				    anaparecer,
				    esdid,
				    esddsc,
				    muncod,
				    anaanalisado
				from(
				SELECT distinct
				      t.turdsc, tr.tirdescricao, 
				      t.turid,
				      n.anaanalisado,
				      ntw.docid,
				      CASE WHEN t.ttuid = 1 THEN 'Exclusivo Creche'
				           WHEN t.ttuid = 2 THEN 'Exclusivo Pré-escola'
				           WHEN t.ttuid = 3 THEN 'Creche e Pré-escola (Misto)'
				      END as tipoatendimento,   
				      t.entcodent,  
				      lt.lotdsc,  
				      t.turdtinicio,
				      doc.esdid,
				      esd.esddsc,
				      (select to_char(min(h.htddata), 'YYYY-MM-DD') from workflow.historicodocumento h
                            inner join workflow.acaoestadodoc ad on ad.aedid = h.aedid  
                        where ad.esdidorigem = ".WF_NOVASTURMAS_EM_CADASTRAMENTO." and ad.esdiddestino = ".WF_NOVASTURMAS_EM_ANALISE." and h.docid = ntw.docid) as dtanalise,
				       case when n.anatipo = 'A' then 'Aprovado'
				          when n.anatipo = 'I' then 'Indeferida'
				          when n.anatipo = 'D' then 'Diligenciada'
				      else '<span style=\"color: red;\">Não Analisado</span>' end as tipoanalise,
				      n.anaid,
				      n.anatipo,
				      n.anaparecer,
				      t.muncod
				FROM proinfantil.turma t
				    inner join proinfantil.tiporede tr ON tr.tirid = t.tirid
				    inner join proinfantil.novasturmasworkflowturma ntw on ntw.turid = t.turid
				    inner join workflow.documento doc on doc.docid = ntw.docid
				    inner join workflow.estadodocumento esd on esd.esdid = doc.esdid
				    left join proinfantil.lotenovasturmas lt on lt.lotid = t.lotid and lt.lotstatus = 'A'
				    left join proinfantil.analisenovasturmasaprovacao n ON n.turid = t.turid and n.anastatus = 'A'
				    
				WHERE 
				    t.muncod = '{$this->muncod}' 
				    and t.turstatus = 'A'
				    and t.turano = '{$this->anoExercicio}'
				    and doc.esdid != ".WF_NOVASTURMAS_EM_CADASTRAMENTO."
				    $filtro
				ORDER BY tr.tirdescricao
				) as foo
				WHERE
					true
				";
		
		
		if($whereAba)
		{
			if( $whereAba == 'abaAnalisados' )
			{
				$sql .="
					AND tipoanalise not in ( '<span style=\"color: red;\">Não Analisado</span>')

					AND
						CASE WHEN tipoanalise = 'Diligenciada' THEN
						
							anaanalisado = true
						ELSE
							true
						END
				";
			}
			else if( $whereAba == 'abaAguardandoAnalise' )
			{
				$sql .="
					AND tipoanalise in ( '<span style=\"color: red;\">Não Analisado</span>', 'Diligenciada')
					and esdid not in (".WF_NOVASTURMAS_EM_DILIGENCIA.")
					AND
					CASE WHEN tipoanalise = 'Diligenciada' THEN
					
						(anaanalisado = false  OR  anaanalisado is null)
					ELSE
						true
					END
					
				";
				
			}
		}
		
		$arrTurmas = $db->carregar( $sql );
		$arrTurmas = $arrTurmas ? $arrTurmas : array();
		
		$arrRegistro = array();
		foreach ($arrTurmas as $key => $v) {
			
			$obNovasTurmas1 = new NovasTurmas( array('turid' => $v['turid'], 'muncod' => $v['muncod']) );
			$aryRepasse = $obNovasTurmas1->carregaRepassePorTurma();
			$aryRepasse = $aryRepasse ? $aryRepasse : array();
			
			$valor_geral = 0;
			$valorTotal = 0;
			foreach ($aryRepasse as $repasse) {
				$valorTotal += ($valorTotal + $repasse['valor_total']);
				$totalGeral = str_replace(".","", $repasse['valor_total']);
				$totalGeral = str_replace(",",".", $totalGeral);
			    
			    $valor_geral += $totalGeral; 
			}
			
			$arrRegistro[$key] = array(
									'turid' 			=> $v['turid'],
									'docid' 			=> $v['docid'],
									'turdsc' 			=> $v['turdsc'],
									'tirdescricao' 		=> $v['tirdescricao'],
									'tipoatendimento' 	=> $v['tipoatendimento'],
									'entcodent' 		=> $v['entcodent'],
									'turdtinicio' 		=> ($v['turdtinicio'] ? formata_data($v['turdtinicio']) : ''),
									'dtanalise' 		=> ($v['dtanalise'] ? formata_data($v['dtanalise']) : ''),
									'periodo' 			=> $aryRepasse[0]['periodo'],
									'valor_total' 		=> number_format($valor_geral, 2, ',', '.'),
									'valor_repasse' 	=> $arrRepasse['valor_repasse'],
									'tipoanalise' 		=> $v['tipoanalise'],
									'anatipo' 			=> $v['anatipo'],
									'anaid' 			=> $v['anaid'],
									'lote' 				=> ($v['lotdsc'] ? $v['lotdsc'] : '-'),
									'anaparecer' 		=> $v['anaparecer'],
									'esdid' 			=> $v['esdid'],
									'esddsc' 			=> $v['esddsc'],
									'muncod' 			=> $v['muncod']
								);
		}
		
		return $arrRegistro;
		//return $arrTurmas;
	}
	
	public function carregaRepassePorTurma(){
		global $db;
		
		//$datacenso = dataCenso();
		
		$sql = "select 
					nome,
				    vrtvalor as vaavalor,
				    anatipo,
				    dataenvio,
				    turdtinicio,
				    total_alunos
				from(
				  SELECT 
				      (CASE WHEN mds.timid = 7 THEN 'Pré-Escola' ELSE 'Creche' END) || ' ' || (CASE WHEN mds.tatid = 1 THEN 'Integral' ELSE 'Parcial' END) AS nome,
				      case when tur.ttuid = 3 then
                      		(select vr.vrtvalor from proinfantil.valorreferencianovasturmas vr 
                            	where vr.tatid = mds.tatid  and vr.ttuid = (case when mds.timid = 7 then 2 else 1 end) 
                            		and vr.tirid = tur.tirid and vr.vrtano = '{$this->anoExercicio}')
                      else
                      	pve.vrtvalor end as vrtvalor,
				      n.anatipo,
				      tur.turdtinicio,
				      (select to_char(min(h.htddata), 'YYYY-MM-DD') from workflow.historicodocumento h
                            inner join workflow.acaoestadodoc ad on ad.aedid = h.aedid  
                        where ad.esdidorigem = ".WF_NOVASTURMAS_EM_CADASTRAMENTO." and ad.esdiddestino = ".WF_NOVASTURMAS_EM_ANALISE." and h.docid = ntw.docid) as dataenvio,
				      ala.total_alunos
				  FROM
				      proinfantil.turma tur
				      inner join proinfantil.novasturmasalunoatendido mds on mds.turid = tur.turid
				      inner join proinfantil.novasturmasworkflowturma ntw on ntw.turid = tur.turid
				      inner join (SELECT ntaid, sum(ntaquantidade) AS total_alunos FROM proinfantil.novasturmasalunoatendido GROUP BY ntaid) as ala on ala.ntaid = mds.ntaid
				      left join proinfantil.valorreferencianovasturmas pve on pve.tatid = mds.tatid and pve.tirid = tur.tirid and pve.ttuid = tur.ttuid and pve.vrtstatus = 'A' and pve.vrtano = '{$this->anoExercicio}'
				      left join proinfantil.analisenovasturmasaprovacao n ON n.turid = tur.turid and n.anastatus = 'A'
				  WHERE
				      tur.muncod = '{$this->muncod}'
				      and tur.turstatus = 'A'
				      and tur.turano = '{$this->anoExercicio}'
				      and tur.turid = {$this->turid}
				  ORDER BY
				      mds.timid DESC, 
				      mds.tatid
				) as foo";
		
		$arrDados = $db->carregar($sql);
		$arrDados = $arrDados ? $arrDados : array();
		
		$arrRegistro = array();
		foreach ($arrDados as $key => $v) {
			
			$arrDados = array(
							'dtinicio' 		=> $v['turdtinicio'],
							'dtanalise' 	=> $v['dataenvio'],
							'pvevalor' 		=> $v['vaavalor'],
							'total_alunos' 	=> $v['total_alunos'],
							'anatipo' 		=> $v['anatipo']
							);
					
			$arrRepasse = calculaDadosRepasse($arrDados);			
			
			//if( $v['total_alunos'] <> 0  ) $valorTotal = ($arrRepasse['saldolimite'] * $v['vaavalor'] );
			 
			//$valorTotal = ( (( (float)$v['vaavalor'] / (int)$arrRepasse['qtdlimite']) * (int)$arrRepasse['saldolimite']) * (int)$v['total_alunos']);
			
			$valorTotal = ((float)$v['vaavalor'] / 12);
			$valorTotal = ($valorTotal * (int)$arrRepasse['saldolimite'] );
			$valorTotal = ( $valorTotal * (int)$v['total_alunos']);
			
			$arrRegistro[$key] = array(
									'nome' 			=> $v['nome'],
									'vaavalor' 		=> number_format($v['vaavalor'], 2, ',', '.'),
									'anatipo' 		=> $v['anatipo'],
									'total_alunos' 	=> $v['total_alunos'],
									'turdtinicio' 	=> ($v['turdtinicio'] ? formata_data($v['turdtinicio']) : ''),
									'dataenvio' 	=> ($v['dataenvio'] ? formata_data($v['dataenvio']) : ''),
									'periodo' 		=> $arrRepasse['saldolimite'],
									'valor_total' 	=> number_format($valorTotal, 2, ',', '.'),
									'valor_repasse' => $arrRepasse['valor_repasse'],
									'tipoanalise' 	=> $v['tipoanalise'],
									'anatipo' 		=> $v['anatipo'],
									'anaparecer' 	=> $v['anaparecer'],
								);
		}
		
		return $arrRegistro;
	}
}
?>