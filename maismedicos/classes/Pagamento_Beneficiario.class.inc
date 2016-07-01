<?php

class Pagamento_Beneficiario extends Modelo{
	
	/**
	 * Nome da tabela especificada
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "maismedicos.pagamento_beneficiario";
	
	/**
	 * Chave primaria.
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array( "benid" );
	
	/**
	 * Atributos
	 * @var array
	 * @access protected
	*/
	protected $arAtributos     = array(
								  'benid' => null,
								  'uniid' => null,
								  'ifeid' => null,
								  'ifcid' => null,
								  'funid' => null,
								  'bencpf' => null,
								  'bennome' => null,
								  'bendatanascimento' => null,
								  'benemail' => null,
								  'bentelefone' => null,
								  'bennomemae' => null,
								  'bencep' => null,
								  'benlogradouro' => null,
								  'bennumero' => null,
								  'bencomplemento' => null,
								  'benbairro' => null,
								  'estuf' => null,
								  'muncod' => null,
								  'bncid' => null,
								  'benagencia' => null,
								  'benconta' => null,
								  'benstatus' => null,  
								  'benvalidade' => null,
								  'bendataatualizacao' => null,
								  'bennumbeneficio' => null,
									);
	
	public function recuperaTutorSupervisorPorCpf($cpf = null)
	{
		if($cpf){
			
			$cpf = str_replace(array('.','-'), '', $cpf);
			
			$sql = "select 
						t.tutnome as nome, 
						t.tutcpf as cpf,
						t.tuttipo as tipo,
						t.tutemail as email,
						to_char(t.tutdatanascimento,'DD/MM/YYYY') as dtnasc,
						t.tuttelefone as fone,
						t.tutnomemae as nomemae,
						t.tutcep as cep,
						t.tutlogradouro as logradouro,
						t.tutnumero as numero,
						t.tutcomplemento as complemento,
						t.tutbairro as bairro,
						t.estuf as uf,
						t.muncod as ibge,
						t.bncid as banco,
						t.tutagencia as agencia,
						t.tutconta as conta,
						t.tutnumbeneficio as beneficio,
						t.uniid,
						u.uninome
					from maismedicos.tutor t
					left join maismedicos.universidade u on u.uniid = t.uniid
					where tutcpf = '{$cpf}' 
					order by tutid desc";
			
			$rs = $this->pegaLinha($sql);
			
			if($rs){
				$rs['nome'] = utf8_encode($rs['nome']);
				$rs['tutnomemae'] = utf8_encode($rs['tutnomemae']);
				$rs['logradouro'] = utf8_encode($rs['logradouro']);
				return $rs;
			}
		}
		
		return false;
	}
	
	public function recuperaBeneficiarioPorCpf($cpf = null)
	{
		if($cpf){
			$cpf = str_replace(array('.','-'), '', $cpf);
			return $this->pegaLinha("select * from {$this->stNomeTabela} where bencpf = '{$cpf}'");
		}
		return false;
		
	}
	
	public function recuperaMedicoPorCpf($cpf = null)
	{
		if($cpf){
			
			$cpf = str_replace(array('.','-'), '', $cpf);
			
			$sql = "select 
						m.mdcnome as nome, 
						m.mdccpf as cpf,
						'M' as tipo,
						m.mdcemail as email,
						m.mdcfone as fone,
						to_char(m.mdcnasc, 'DD/MM/YYYY') as dtnasc,
						m.mdcfone as fone,
						'' as nomemae,
						'' as cep,
						'' as logradouro,
						'' as numero,
						'' as complemento,
						'' as bairro,
						m.estuf as uf,
						m.muncod as ibge,
						'' as banco,
						'' as agencia,
						'' as conta, 
						'' as beneficio,
						m.uniid,
						u.uninome
					from maismedicos.medico m
					left join maismedicos.universidade u on u.uniid = m.uniid
					where mdccpf = '{$cpf}' 
					order by mdcid desc";
			
			$rs = $this->pegaLinha($sql);
			
			if($rs){
				$rs['nome'] = utf8_encode($rs['nome']);
				return $rs;
			}
		}
		
		return false;
	}
	
	function recuperaBeneficiario($post)
	{
		if($post['benid']){
			$arWhere[] = "benid = {$post['benid']}";
		}
		if($post['bencpf']){
			$arWhere[] = "bencpf = {$post['bencpf']}";
		}
		$sql = "select b.*, u.uninome, b.ifeid, b.ifcid, i.ifenome from maismedicos.pagamento_beneficiario b
				left join maismedicos.universidade u on u.uniid = b.uniid
				left join maismedicos.ifes i on i.ifeid = b.ifeid
				left join maismedicos.ifes_campi c on c.ifcid = b.ifcid
				where benstatus = 'A'".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '');
		
		return $this->pegaLinha($sql);
	}
	
}