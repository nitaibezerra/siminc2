<?php
class CronogramaExecucao extends cls_banco {
	private $param;
	 
	public function __construct(){
		parent::__construct();
	}
	public function __destruct(){
		if( isset($_SESSION['transacao'] ) ){
			pg_query( $this->link, 'rollback; ');
			unset( $_SESSION['transacao'] );
		}
	}
	
	public function __set( $prop, $valor ){
		$this->param[$prop] = $valor; 
	}
	
	public function __get( $prop ){
		return $this->param[$prop];
	}	
	
	public function manterCronogramaExecucao( $post, $obPTA, $boReformulacao = false ){
		$arPrivalor = ( $post["privalor"] ? $post["privalor"] : array() );
			
		$arPrdid = array();
		$iniciativas = array();

		foreach($arPrivalor as $chave=>$valor) { 
			$prdtipo = $post['prdtipo'][$chave];
			$valor = str_replace(".", "", $valor);
			$valor = (float)str_replace(",", ".", $valor);
			
			/*if( $post['prdminuta'] == 'P' ){
				$iniciativas = $this->carregaIniciativaCronograma();
				if( $post['dataP'][$index] ){
					$data = $data = explode("/", $post['dataP'][$index]);
					$data = $data[1]."-".$data[0]."-01";
				} else {
					$data = explode("/", $post['datainicial']);
					$data = $data[1]."-".$data[0]."-01";
				}
				
				$sql = "UPDATE emenda.ptparceladesembolso SET ptrid = ".$this->ptrid.", prddata = '".$data."'
						WHERE prdid = :prdid";
				
				//$arPrdid[] = $this->criarParcelaDesembolso( $this->ptrid, $data, $prdtipo, 'P', $iniciativas );
			} else {*/				
				$sql = "UPDATE 
						emenda.ptparcelainiciativa 
					SET 
						privalor = ".$valor."
					WHERE
						priid = ".$chave;
				parent::executar($sql);
			//}
		}
		/*if( $post['prdminuta'] == 'P' && empty($post['ptridpai']) && !empty($post['refid']) ){
			$ptrid = $obPTA->insereFilhosPTA( '', $this->ptrid, '', '', $arPrdid );
			$_SESSION["emenda"]["ptridpai"] = $this->ptrid;
		}*/
		if( $post['prdminuta'] == 'P' ){
			if( $_SESSION['exercicio'] != '2009' ){
				$post['prddata'] = $post['prddata'] ? $post['prddata'] : array();
				foreach ($post['prddata'] as $key => $v) {
					$data = $data = explode("/", $post['prddata'][$key]);
					$data = $data[1]."-".$data[0]."-01";
					
					$sql = "UPDATE 
							  	emenda.ptparceladesembolso  
							SET 
							  	prddata = '".$data."'
							WHERE 
							  	prdid = $key";
					
					parent::executar($sql);
				}			
			}
		}
		
		parent::commit();
		if($post['tipoChamada']){
			$chamada = '&chamada='.$this->tipoChamada;
			$opener = "window.opener.$('boCronograma').value = 'true';";
		} else {
			$chamada = '';
			$opener = '';
		}
		if( $boReformulacao ){
			echo "<script>window.opener.location = 'emenda.php?modulo=principal/reformulacaoPTA&acao=A&ptrid=".$this->ptrid."'; </script>";
			parent::sucesso('principal/cronogramaDesembolsoReformulacao','&ptrid='.$this->ptrid);
		} else{
			echo "<script>$opener</script>";
			parent::sucesso('principal/cronogramaExecuccaoDesembolso', $chamada);
		}
		exit();
	}
	
	public function criarParcelaDesembolso($ptrid, $data, $tipo, $prdminuta, $arIniciativas = array()) {
		
		//$ptridNovo = ( $tipoInsert == 'filho' ? $ptridNovo : $ptrid );
		
		$sql = "INSERT INTO emenda.ptparceladesembolso(ptrid, prddata, prdtipo, prdminuta)
				VALUES (".$ptrid.", '".$data."', '".$tipo."', '".$prdminuta."')
				RETURNING prdid";
	
		$prdid = parent::pegaUm($sql);
		
		if( $prdminuta == 'P' ){
			if( $arIniciativas ){
				//grava o valor total da iniciativa na primeira parcela
				foreach ($arIniciativas as $key => $v) {
					$sql = "SELECT pti.ptiid, pti.ptivalorconcedente, pti.ptivalorproponente
							FROM emenda.v_ptiniciativa pti
							WHERE pti.ptrid = $ptrid and pti.ptiid = {$v['ptiid']} ";
					$valorIniciativa = parent::carregar( $sql );
									
					if( $valorIniciativa ){
						foreach ($valorIniciativa as $valor) {
							if( $tipo == 'C' ){
								$ptivalorconcedente = $valor['ptivalorconcedente'];
								$ptiid = $valor['ptiid'];
								$this->criarParcelaIniciativa($prdid, $ptrid, $ptivalorconcedente, $ptiid); 	
							} else {	
								$ptiid = $valor['ptiid'];
								$ptivalorproponente = $valor['ptivalorproponente'];						
								$this->criarParcelaIniciativa($prdid, $ptrid, $ptivalorproponente, $ptiid);	
							}
						}
					}
				}
			}else {
				$this->criarParcelaIniciativa($prdid, $ptrid);		
			}		 
		} else {
			$this->criarParcelaIniciativa($prdid, $ptrid);
		}
		parent::commit();
		return $prdid;
	}
	
	public function criarParcelaIniciativa($prdid, $ptrid, $privalor = null, $ptiid = null ) {
		
		$filtro = ( $ptiid ? "and ptiid = $ptiid" : '' );
		
		$privalor = ( $privalor ? $privalor : 'null' );
		
		$sql = "INSERT INTO emenda.ptparcelainiciativa(prdid, ptiid, privalor) 
				SELECT ".$prdid.",ptiid, ".$privalor." FROM emenda.ptiniciativa WHERE ptrid = ".$ptrid." $filtro";
		parent::executar($sql);
	}
	
	public function excluiCronogramaDesembolso( $post ){
		$sql = "DELETE FROM emenda.ptparcelainiciativa WHERE prdid = ".$post["parcela_excluir"];
		parent::executar($sql);
		
		$sql = "DELETE FROM emenda.ptparceladesembolso 
				WHERE prdid = ".$post["parcela_excluir"]." AND 
					  prdtipo = '".$post["tipo_parcela_excluir"]."'";
		parent::executar($sql);
		parent::commit();
		
		if($post['chamada']){
			$chamada = 'chamada='.$this->tipoChamada;
		} else {
			$chamada = '';
		}
		
		echo "<script type=\"text/javascript\">
				alert(\"Parcela excluída com sucesso.\");
				window.location = \"emenda.php?modulo=principal/cronogramaExecuccaoDesembolso&acao=A&$chamada\&excluir=sim\";
			  </script>";
		die;
	}
	
	public function incluirParcelasInexistentes() {
			
		$sql = "Insert into emenda.ptparcelainiciativa (prdid, ptiid)
			Select prd.prdid, pti.ptiid
			From emenda.planotrabalho ptr
			 inner join emenda.ptiniciativa pti
			  on ptr.ptrid = pti.ptrid
			 inner join emenda.ptparceladesembolso prd
			  on prd.ptrid = ptr.ptrid
			  AND prdminuta = '".$this->prdminuta."'
			 left join emenda.ptparcelainiciativa pri
			  on pti.ptiid = pri.ptiid
			  and pri.prdid = prd.prdid
			Where ptr.ptrid = ".$this->ptrid."
			 and pri.priid is null";
	
		parent::executar($sql);
		parent::commit();
	}
		
	public function carregaIniciativaCronograma(){
		
		// carrega as iniciativas
		$sql = "SELECT DISTINCT
					pti.ptiid,	
					ini.ininome
				FROM
					emenda.iniciativa ini
				INNER JOIN
					emenda.ptiniciativa pti ON pti.iniid = ini.iniid
										   AND pti.ptrid = ".$this->ptrid."
				/*WHERE
					ini.inistatus = 'A'*/";
		
		$iniciativas = parent::carregar($sql);
		$iniciativas = ( $iniciativas ? $iniciativas : array() );
		return $iniciativas;
	}

	public function verificaIniciativaVinculadaEspecificacao( $boReformulacao = false ){
		$sql = "SELECT
					to_char(min(pte.ptedatainicio), 'MM/YYYY') as inicial,
					to_char(max(pte.ptedatafim), 'MM/YYYY') as final
				FROM
					emenda.ptiniciativaespecificacao pte
				INNER JOIN
					emenda.ptiniciativa pti ON pti.ptiid = pte.ptiid
										   AND pti.ptrid = ".$this->ptrid."
				WHERE
					ptestatus = 'A'";
		
		$cronogramaExecucao = parent::carregar($sql);
				
		if( (!$cronogramaExecucao[0]["inicial"] || !$cronogramaExecucao[0]["final"]) && !$boReformulacao ) {
			echo "<script>
					alert('Antes de acessar o cronograma é necessário cadastrar as iniciativas e suas especificações.');
					history.back(-1);
				  </script>";
			die;
		} else {
			return $cronogramaExecucao;
		}
	}
	
	public function carregaParcelaCronograma($iniciativa, $prdminuta, $tipo){
		$sql = "SELECT
					pti.ptiid,
					pri.priid,
					pri.privalor,
					pti.ptivalorconcedente,
					pti.ptivalorproponente
				FROM
					emenda.v_ptiniciativa pti
				LEFT JOIN
					emenda.ptparcelainiciativa pri 
					INNER JOIN emenda.ptparceladesembolso 
	                	prd ON prd.prdid = pri.prdid
						AND prd.prdtipo = '$tipo'
						AND prd.prdminuta = '$prdminuta'
	                ON pri.ptiid = pti.ptiid
				WHERE
					pti.ptiid = ".$iniciativa."
				ORDER BY
					prd.prddata ASC";
		
		return parent::carregar($sql);
	}
	
	public function carregaTipoParcelaDesembolso(){
		$sql = "SELECT DISTINCT
			 prdtipo
		FROM 
			emenda.ptparceladesembolso prd
		WHERE 
			ptrid = ".$this->ptrid." 
			AND prdminuta = '".$this->prdminuta."'";
		$verificaParcelaDesembolso = parent::carregar($sql);
		$verificaParcelaDesembolso = $verificaParcelaDesembolso ? $verificaParcelaDesembolso : array();
		
		return $verificaParcelaDesembolso; 
	}
	
	/**
	 * Carrega a quantidade de parcela da minuta e do pagamento
	 *
	 * @param integer $ptrid
	 * @return record
	 */
	public function verificaParcelaMinutaPagamento(){
		$sql = "SELECT 
					 count(ppd.prdid)
					FROM 
					  emenda.ptparceladesembolso ppd
					WHERE
					  ppd.ptrid = $this->ptrid
					  and ppd.prdminuta = 'G'
	                  union
				SELECT 
					  count(ppd.prdid)
					FROM 
					  emenda.ptparceladesembolso ppd
					WHERE
					  ppd.ptrid = $this->ptrid
					  and ppd.prdminuta = 'M'";
		
		return parent::carregarColuna( $sql );
	}
}
?>