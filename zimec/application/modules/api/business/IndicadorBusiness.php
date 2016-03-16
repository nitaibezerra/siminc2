<?php
class Api_Business_IndicadorBusiness
{
	private $modelUsuario;
	
	private $modelPostgres;
	
	public function __construct()
	{
		$this->modelDemanda = new Model_Demandas_Demanda();
		$this->modelUsuario = new Model_Seguranca_Usuario();
		$this->modelPostgres = new Model_Public_Postgres();
	}
	
	public function listaDemandas()
	{
		$usuarios = $this->modelUsuario->getUsuarios();
		
		foreach ($usuarios as $usuario) 
		{
			$demandas = $this->modelDemanda->getDemandasByCPF($usuario['usucpf']);
			
			var_dump($demandas);
			die;
		}
		
		$tempo = $this->modelPostgres->getTempoQuery();
	
		return (int) $tempo['segundos'];
	}
	
	public function totalTempoExcucao()
	{
		$tempo = $this->modelPostgres->getTempoQuery();
	
		return (int) $tempo['segundos'];
	}
	
	public function totalQueryExcucao()
	{
		return count($this->modelPostgres->getQuantidadeQuery());
	}
	
	public function totalUsuariosOnline()
	{
		$quantidade = $this->modelUsuario->getUsuarioOnline();
	
		return (int) $quantidade['online'];
	}

}