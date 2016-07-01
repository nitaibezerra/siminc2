<?php
	
class Atividade extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "gestaodocumentos.atividade";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "atvid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'atvid' => null,
									  	'tarid' => null, 
									  	'usucpf' => null, 
									  	'atvdetalhe' => null, 
									  	'atvdtinclusao' => null, 
									  	'atvstatus' => null, 
									  	'atvhistworflow' => null
									  );
									  
	public function salvarAtividade($post){
        extract($post);
        $docid = buscarDocidGestaoDocumentos($tarid);
        $atvhistworflow = $this->historicoWorkflow($docid);

        $sql_pos = "INSERT INTO gestaodocumentos.atividade (
					tarid,
					usucpf,
					atvdetalhe,
					atvdtinclusao,
					atvstatus,
					atvhistworflow
				) VALUES (
					{$tarid},
					'{$_SESSION['usucpf']}',
					'{$atvdetalhe}',
					now(),
					'A',
					'{$atvhistworflow}'
				) RETURNING atvid;";

        unset( $_POST['atvdetalhe'] );

        foreach( $_POST as $chave => $item ){
            if( strpos($chave, 'atvid') !== false ){
                $_POST['atvid'][] = $item;
                unset($_POST[$chave]);
            }
            if( strpos($chave, 'atvdetalhe') !== false ){
                $_POST['atvdetalhe'][] = $item;
                unset($_POST[$chave]);
            }
        }
        //ver($_POST,d);

        $sql = "";
        foreach( $_POST['atvid'] as $index => $value ){
            $sql .= "UPDATE gestaodocumentos.atividade
				SET    	atvdetalhe = '{$_POST['atvdetalhe'][$index]}'
				WHERE 	atvid = {$value};";
        }
        $sql .= $sql_pos;
        // exit($sql);

        $atvid = $this->pegaUm($sql);
        $this->commit();
        return $atvid;
    }

	public function alterarAtividade($post){
		extract($post);
		$sql = "UPDATE 	gestaodocumentos.atividade 
				SET    	atvdetalhe = '{$atvdetalhe}'
				WHERE 	atvid = {$atvid};";

		$this->executar($sql);
		$this->commit();
	}

    public function alterarAtividade2($post){
        foreach( $post as $chave => $item ){
            if( strpos($chave, 'atvid') !== false ){
                $post['atvid'][] = $item;
                unset($post[$chave]);
            }
            if( strpos($chave, 'atvdetalhe') !== false ){
                $post['atvdetalhe'][] = $item;
                unset($post[$chave]);
            }
        }
        $sql = "";
        foreach( $post['atvid'] as $index => $value ){
            $sql .= "UPDATE gestaodocumentos.atividade
				SET    	atvdetalhe = '{$post['atvdetalhe'][$index]}'
				WHERE 	atvid = {$value};";
        }
        $this->executar($sql);
        $this->commit();
    }
	
	public function excluirAtividade($post){
		extract($post);
		$sql = "UPDATE gestaodocumentos.atividade 
				SET    atvstatus = 'I'
				WHERE  atvid= {$atvid}";
		
		$this->executar($sql);
		$this->commit();
	}	

	public function visualizarAtividade($post){
		extract($post);
		$sql = "SELECT 		atvid, atvdetalhe
  				FROM 		gestaodocumentos.atividade
				WHERE 		atvid = {$atvid}";
		$rs = $this->pegaLinha($sql);
		return $rs;
	}		

	public function historicoWorkflow($docid){
		$sql = "SELECT 			eo.esddsc || ' => ' || ed.esddsc AS esdiddestino 
				FROM 			workflow.acaoestadodoc ac 
				INNER JOIN 		workflow.estadodocumento eo ON eo.esdid = ac.esdidorigem
				INNER JOIN 		workflow.estadodocumento ed ON ed.esdid = ac.esdiddestino 
				WHERE 			ac.aedid = (SELECT aedid FROM workflow.historicodocumento WHERE docid = {$docid} ORDER BY hstid DESC LIMIT 1)";
		
		$rs = $this->pegaUm($sql);
		return $rs;
	}
}