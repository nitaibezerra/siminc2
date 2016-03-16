<?php
	
class Supervisao_Os_Obra extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.supervisao_os_obra";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "sooid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'sooid' => null, 
									  	'sosid' => null, 
									  	'empid' => null, 
									  	'soostatus' => null, 
									  );

	public function listaEmpidPorOs( $sosid , $cancelada = false){
		$sosid = ( $sosid ? $sosid : 0 );

        $sql = "SELECT
					empid
				FROM
					obras2.supervisao_os_obra
				WHERE
					soostatus = 'A' AND
					sosid = {$sosid}";

        if($cancelada)
            $sql = "SELECT soo.empid FROM obras2.supervisao_os os
                JOIN obras2.supervisao_os_obra soo ON soo.sosid = os.sosid AND soo.soostatus = 'A'
                JOIN obras2.supervisaoempresa sue ON sue.empid = soo.empid AND sue.sosid = os.sosid AND sue.suestatus = 'A'
                JOIN workflow.documento d ON d.docid = sue.docid
                WHERE os.sosid = {$sosid} AND d.esdid != 1188";


		$dado = $this->carregarColuna( $sql );
		return ($dado ? $dado : array());
	}
	
	public function apagaPorSosid( $sosid ){
		$sql = "UPDATE obras2.supervisao_os_obra SET soostatus = 'I' WHERE sosid = {$sosid}";
		$this->executar( $sql );
	}
	
	public function geraNovoNumOS()
	{
		$sql = "select sosid +1 as sosid from $this->stNomeTabela order by sosid desc limit 1";
		$sosid =  $this->pegaUm($sql);
		if(!$sosid){
			$sosid = 1;
		}
		return $sosid;
	}

    public function pegaSituacaoSupervisao($sosid, $empid, $obrid = null)
    {
        $where = ($obrid) ? "AND o.obrid = $obrid" : "AND soo.empid IN ($empid)";
        $sql = "
            SELECT
                o.obrid,
                soo.empid,
                e.esdid,
                e.esddsc
            FROM obras2.supervisao_os os
            JOIN obras2.supervisao_os_obra soo ON soo.sosid = os.sosid AND soo.soostatus = 'A'
            JOIN obras2.obras o ON o.empid = soo.empid AND o.obridpai IS NULL AND o.obrstatus = 'A'
            JOIN obras2.supervisaoempresa sue ON sue.sosid = os.sosid AND soo.empid = sue.empid AND sue.suestatus = 'A'
            JOIN workflow.documento d ON d.docid = sue.docid
            JOIN workflow.estadodocumento e ON e.esdid = d.esdid
            WHERE
                os.sosid = $sosid
                $where
                AND os.sosstatus = 'A'
	  ";
       return $this->pegaLinha($sql);
    }
}