<?php
	
class PreObraAnexo extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras.preobraanexo";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "poaid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'poaid' => null, 
									  	'preid' => null, 
									  	'poadescricao' => null,									  	 
									  	'arqid' => null,									  	 
									  	'podid' => null,
									    'datainclusao' => null,
									    'usucpf' => null,
									    'poasituacao' => null									  	 
									  );
									  
	public function listaDocumentos()
	{
		$sql = "SELECT
                	to_char(arq.arqdata,'DD/MM/YYYY'),
                    pod.poddescricao,
                    arq.arqnome || '.'|| arq.arqextensao as nome,
                    arq.arqtamanho || ' kbs' as tamanho ,
                    arq.arqdescricao,                               
                    usu.usunome,
                    arq.arqid
               	FROM public.arquivo arq 
                    INNER JOIN obras.preobraanexo     poa ON arq.arqid = poa.arqid AND poa.poasituacao = 'A'
                    INNER JOIN obras.pretipodocumento pod ON pod.podid = poa.podid
					INNER JOIN seguranca.usuario      usu ON usu.usucpf = arq.usucpf
                WHERE
                    arq.arqstatus = 'A'
                    AND poa.preid = '{$_SESSION['par']['preid']}' ";
		return $this->carregar($sql);
	}
	
	public function excluirDocumento($arqid)
	{
		$sql = "UPDATE obras.preobraanexo SET poasituacao = 'I'  WHERE arqid=".$arqid." AND preid = ".$_SESSION['brasilpro']['preid'] ;
	    $this->executar($sql);
	    $sql = "UPDATE public.arquivo SET arqstatus = 'I' WHERE arqid=".$arqid;
	    $this->executar($sql);
	
	    $this->commit();
	} 
	
	public function recuperarPrefeitoMunicipio($muncod, $funid = 2)
	{
		if($funid == 2){
			$sql = "select
						e.entid,
						e.entnome,
						e.entnumcpfcnpj,
						mun.muncod,
						mun.estuf,
						mun.mundescricao,
						eed2.endlog,
						eed2.endbai,
						e.entnumrg,
						e.entorgaoexpedidor,
						est.estdescricao,
						e.entstatus
					from entidade.entidade e
					inner join entidade.funcaoentidade fe on e.entid = fe.entid AND fe.funid = 2
					INNER JOIN entidade.funentassoc feaprefeito ON feaprefeito.fueid = fe.fueid 
					LEFT JOIN entidade.entidade         ent2 ON ent2.entid = feaprefeito.entid 
					LEFT JOIN entidade.funcaoentidade   fue2 ON fue2.entid = ent2.entid AND fue2.funid = 1 AND fue2.fuestatus = 'A'
					LEFT JOIN entidade.funcao 	    fun2 ON fun2.funid = fue2.funid
					LEFT JOIN entidade.endereco         eed2 ON eed2.entid = ent2.entid 
					INNER JOIN territorios.municipio mun ON eed2.muncod = mun.muncod
					INNER JOIN territorios.estado est ON est.estuf = mun.estuf 
					where eed2.muncod = '{$muncod}'";
		}else{
			$sql = "select
						e.entid,
						e.entnome,
						e.entnumcpfcnpj,
						mun.muncod,
						mun.estuf,
						mun.mundescricao,
						ed.endlog,
						ed.endbai,
						e.entnumrg,
						e.entorgaoexpedidor,
						est.estdescricao
					from entidade.entidade e
					inner join entidade.funcaoentidade fe on e.entid = fe.entid 
					inner join entidade.endereco ed on e.entid = ed.entid 
					INNER JOIN territorios.municipio mun ON ed.muncod = mun.muncod
					INNER JOIN territorios.estado est ON est.estuf = mun.estuf 
					where ed.muncod = '{$muncod}'
					and fe.funid = {$funid}";
		}
		
		return $this->pegaLinha($sql);
	} 
	
	public function recuperarSecretarioMunicipio($estuf, $funid2 = '25')
	{
		if($funid2 == 25){
			$inner = "INNER JOIN entidade.funentassoc feaprefeito ON feaprefeito.fueid = fe.fueid ";
		}else{
			$inner = '';
		}
		
		$sql = "select
					e.entid,
					e.entnome,
					e.entnumcpfcnpj,
					mun.muncod,
					mun.estuf,
					mun.mundescricao,
					ed.endlog,
					ed.endbai,
					e.entnumrg,
					e.entorgaoexpedidor,
					est.estdescricao
				from entidade.entidade e
				inner join entidade.funcaoentidade fe on e.entid = fe.entid 
				".$inner."
				inner join entidade.endereco ed on e.entid = ed.entid 
				INNER JOIN territorios.municipio mun ON ed.muncod = mun.muncod
				INNER JOIN territorios.estado est ON est.estuf = mun.estuf 
				where ed.estuf = '{$estuf}'
				and fe.funid = {$funid2}";
		
		return $this->pegaLinha($sql);
	}
									  
}