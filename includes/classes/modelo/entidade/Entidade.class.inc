<?php
	
class Entidade extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "entidade.entidade";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "entid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'entid' => null, 
									  	'njuid' => null, 
									  	'entnumcpfcnpj' => null, 
									  	'entnome' => null, 
									  	'entemail' => null, 
									  	'entnuninsest' => null, 
									  	'entobs' => null, 
									  	'entstatus' => null, 
									  	'entnumrg' => null, 
									  	'entorgaoexpedidor' => null, 
									  	'entsexo' => null, 
									  	'entdatanasc' => null, 
									  	'entdatainiass' => null, 
									  	'entdatafimass' => null, 
									  	'entnumdddresidencial' => null, 
									  	'entnumresidencial' => null, 
									  	'entnumdddcomercial' => null, 
									  	'entnumramalcomercial' => null, 
									  	'entnumcomercial' => null, 
									  	'entnumdddfax' => null, 
									  	'entnumramalfax' => null, 
									  	'entnumfax' => null, 
									  	'tpctgid' => null, 
									  	'tpcid' => null, 
									  	'tplid' => null, 
									  	'tpsid' => null, 
									  	'entcodentsup' => null, 
									  	'entcodent' => null, 
									  	'entescolanova' => null, 
									  	'entdatainclusao' => null, 
									  	'entsig' => null, 
									  	'entunicod' => null, 
									  	'entungcod' => null, 
									  	'entproep' => null, 
									  	'entnumdddcelular' => null, 
									  	'entnumcelular' => null, 
									  	'entorgcod' => null, 
									  	'entsede' => null, 
									  	'entrazaosocial' => null, 
									  	'entescolaespecializada' => null, 
									  );
									  
	function buscaNomeEntidade($entid){
		$sql = "SELECT
					entnome
				FROM
					{$this->stNomeTabela}
				WHERE
					entid = {$entid}";
					
		return $this->pegaUm( $sql );			
	}

	function buscarUniversidadeByHospital($entid){
		$sql = "SELECT 
					upper(ena.entnome) as entnome, 
					COUNT(ent.entid) as entid,
					ena.entsig as ifes 
				FROM 
					{$this->stNomeTabela} ent 
				LEFT JOIN 
					entidade.funcaoentidade fen ON fen.entid = ent.entid 
				INNER JOIN 
					entidade.funentassoc fue ON fue.fueid = fen.fueid
				LEFT 
					JOIN entidade.entidade ena ON ena.entid = fue.entid
				LEFT JOIN 
					rehuf.estruturaunidade esu ON esu.entid = ent.entid
				WHERE 
					fen.funid IN ('16','93') 
				AND 
					(esu.esuindexibicao IS NULL OR esu.esuindexibicao = true) 
				AND 
					ent.entid = {$entid}  
				GROUP BY ena.entid, ena.entnome, ena.entsig 
				ORDER BY ena.entsig";
			return $this->pegaLinha($sql);
	}
	
	public function buscarentidadeporentungcod($entungcod){
		$sql = "select 
					ent.entid 
				from 
					{$this->stNomeTabela} ent 
				where 
					ent.entungcod = '{$entungcod}'";
		return $this->pegaUm($sql);
	}
	
	public function listaComboObras2(array $param = array()){


    if($param['muncod']){
        $sql = "SELECT DISTINCT
					ent.entid AS codigo,
					entnome AS descricao
				FROM
					{$this->stNomeTabela} ent
				INNER JOIN obras2.obras obr ON obr.entid = ent.entid
                INNER JOIN entidade.endereco ed ON ed.entid = ent.entid
                WHERE ed.muncod = '{$param['muncod']}'
                ";
    } else {
        $sql = "SELECT DISTINCT
					ent.entid AS codigo,
					entnome AS descricao
				FROM
					{$this->stNomeTabela} ent
				INNER JOIN obras2.obras obr ON obr.entid = ent.entid";
    }
    $dados = $this->carregar($sql);

    return (is_array($dados) ? $dados : array());
}
    public function listaComboMulti($param =null){

        $where = array();

        if(!empty($param) && is_array($param)){
        foreach($param as $key) {
            $where[] = "ed.muncod = '{$key}'";
        }
            
        }
        if(!empty($param) && is_array($param)){
        $sql = "SELECT DISTINCT
					ent.entid AS codigo,
					entnome AS descricao
				FROM
					{$this->stNomeTabela} ent
				INNER JOIN obras2.obras obr ON obr.entid = ent.entid
                INNER JOIN entidade.endereco ed ON ed.entid = ent.entid" .
            (count($where) ? " WHERE " . implode(" or ", $where) : "") . "
                    ORDER BY
                            entnome";
        } else {
            $sql = "SELECT DISTINCT
					ent.entid AS codigo,
					entnome AS descricao
				FROM
					{$this->stNomeTabela} ent
				INNER JOIN obras2.obras obr ON obr.entid = ent.entid
				 ORDER BY
                            entnome"
            ;
        }
        $dados = $this->carregar($sql);

        return (is_array($dados) ? $dados : array());
    }
}