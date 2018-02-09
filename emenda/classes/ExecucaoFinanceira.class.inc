<?php
	
class ExecucaoFinanceira extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "emenda.execucaofinanceira";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "exfid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'exfid' => null, 
									  	'tpeid' => null, 
									  	'pliid' => null, 
									  	'ptrid' => null, 
									  	'pedid' => null, 
									  	'exfvalor' => null, 
									  	'exfcodmunicipiosiafi' => null, 
									  	'exfnumsolempenho' => null, 
									  	'exfespecieempenho' => null, 
									  	'exfnumconveniofnde' => null, 
									  	'exfstatus' => null, 
									  	'exfcodfontesiafi' => null, 
									  	'semid' => null, 
									  	'exfdataalteracao' => null, 
									  	'usucpf' => null, 
									  	'exfanooriginal' => null, 
									  	'exfnumempenhooriginal' => null, 
									  	'exfidpai' => null, 
									  	'exfnaturezadespesa' => null, 
									  	'exfverifsiafi' => null, 
									  	'exfverifcadin' => null, 
									  	'exfdatainclusao' => null,
    									'exfdataemisao' => null 
									  );
									  
	public function carregaExecPorPtrid($ptrid){
		$sql = "SELECT distinct
				    ef.exfid,
				    ef.tpeid,
				    ef.pliid,
				    ef.ptrid,
				    ef.pedid,
				    ef.exfvalor,
				    ef.exfcodmunicipiosiafi  || ' - ' || mun.mundescricao as municipiosiafi,
				    ef.exfnumsolempenho,
				    u.usunome,
				    CASE WHEN ef.exfespecieempenho = '01' THEN 'Original'
					 WHEN ef.exfespecieempenho = '02' THEN 'Reforço'
					 WHEN ef.exfespecieempenho = '03' THEN 'Anulação'
				    END AS especieempenho,
				    CASE WHEN ef.exfstatus = 'A' THEN 'Ativo'
					 ELSE 'Inativo'
				    END AS status,
				    ef.exfcodfontesiafi,
				    se.semcod || ' - ' ||se.semdesc as situacao,
				    to_char(ef.exfdataalteracao, 'DD/MM/YYYY HH24:mi:ss') as exfdataalteracao,
				    ef.usucpf,
				    ef.exfanooriginal, 
				    ef.exfnumempenhooriginal,
				    ef.exfidpai,
				    ef.exfnaturezadespesa,
				    CASE WHEN ef.exfverifsiafi = true THEN 'Sim'
					 ELSE 'Não'
				    END AS siafi,
				    CASE WHEN ef.exfverifcadin = true THEN 'Sim'
					 ELSE 'Não'
				    END AS cadin,
				    to_char(ef.exfdatainclusao, 'DD/MM/YYYY HH24:mi:ss') as exfdatainclusao
				FROM emenda.execucaofinanceira ef
				   inner join monitora.pi_planointerno pli on pli.plicod = ef.plicod
				   inner join monitora.pi_planointernoptres plpt on pli.pliid = plpt.pliid
				   inner join monitora.ptres ptr on ptr.ptrid = plpt.ptrid and ptr.ptres = ef.ptres
				   inner join emenda.planotrabalho pt on ef.ptrid = pt.ptrid
				   inner join emenda.tipoensino te on ef.tpeid = te.tpeid
				   inner join emenda.ptemendadetalheentidade ped on ef.pedid = ped.pedid
				   inner join emenda.situacaoempenho se on ef.semid = se.semid 
				   inner join emenda.v_emendadetalheentidade vede on vede.edeid = ped.edeid
				   inner join emenda.entidadebeneficiada eb on eb.enbid = vede.entid 
				   -- inner join entidade.endereco ende on vede.entid = ende.entid
				   inner join territorios.municipio mun on eb.muncod = mun.muncod
				   inner join seguranca.usuario u on ef.usucpf = u.usucpf
				WHERE ef.ptrid = $ptrid
				  --  AND ef.exfidpai is null
				  and ef.exfstatus = 'A'
				order by exfdatainclusao";
		$arDados = $this->carregar($sql);
		$arDados = ($arDados) ? $arDados : array();
		return $arDados;
	}
	
	public function carregaFilhos($exfid){
		$sql = "SELECT 
				    ef.exfid,
				    ef.tpeid,
				    ef.pliid,
				    ef.ptrid,
				    ef.pedid,
				    ef.exfvalor,
				    ef.exfcodmunicipiosiafi  || ' - ' || mun.mundescricao as municipiosiafi,
				    ef.exfnumsolempenho,
				    u.usunome,
				    CASE WHEN ef.exfespecieempenho = '01' THEN 'Original'
					 WHEN ef.exfespecieempenho = '02' THEN 'Reforço'
					 WHEN ef.exfespecieempenho = '03' THEN 'Anulação'
				    END AS especieempenho,
				    CASE WHEN ef.exfstatus = 'A' THEN 'Ativo'
					 ELSE 'Inativo'
				    END AS status,
				    ef.exfcodfontesiafi,
				    se.semcod || ' - ' ||se.semdesc as situacao,
				    to_char(ef.exfdataalteracao, 'DD/MM/YYYY HH24:mi:ss') as exfdataalteracao,
				    ef.usucpf,
				    ef.exfanooriginal, 
				    ef.exfnumempenhooriginal,
				    ef.exfidpai,
				    ef.exfnaturezadespesa,
				    CASE WHEN ef.exfverifsiafi = true THEN 'Sim'
					 ELSE 'Não'
				    END AS siafi,
				    CASE WHEN ef.exfverifcadin = true THEN 'Sim'
					 ELSE 'Não'
				    END AS cadin,
				    to_char(ef.exfdatainclusao, 'DD/MM/YYYY HH24:mi:ss') as exfdatainclusao
				FROM emenda.execucaofinanceira ef
				   inner join monitora.pi_planointerno pi on pi.plicod = ef.plicod
				   inner join monitora.pi_planointernoptres plpt on pi.pliid = plpt.pliid
				   inner join monitora.ptres ptr on ptr.ptrid = plpt.ptrid and ptr.ptres = ef.ptres
				   inner join emenda.planotrabalho pt on ef.ptrid = pt.ptrid
				   inner join emenda.tipoensino te on ef.tpeid = te.tpeid
				   inner join emenda.ptemendadetalheentidade ped on ef.pedid = ped.pedid
				   inner join emenda.situacaoempenho se on ef.semid = se.semid 
				   inner join emenda.v_emendadetalheentidade vede on vede.edeid = ped.edeid
				   -- inner join entidade.endereco ende on vede.enbid = ende.enbid
				   inner join emenda.entidadebeneficiada eb on eb.enbid = vede.enbid
				   inner join territorios.municipio mun on ende.muncod = mun.muncod
				   inner join seguranca.usuario u on ef.usucpf = u.usucpf
				WHERE ef.exfidpai = $exfid
				order by exfdatainclusao";
		$arDados = $this->carregar($sql);
		$arDados = ($arDados) ? $arDados : array();
		//ver($arDados);
		return $arDados;
	}
	
	public function temFilho($exfid){
		$sql = "select count(1) as count from emenda.execucaofinanceira where exfidpai = $exfid ";
		return $this->pegaUm($sql);
	}
}