<?php

include_once APPRAIZ . 'maismedicos/classes/Planotrabalho.class.inc';
	
class Planotrabalho_Execucao extends Planotrabalho{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "maismedicos.planotrabalho_execucao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "pteid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'pteid' => null, 
									  	'perid' => null, 
									  	'ptrid' => null, 
									  	'ptecpf' => null, 
									  	'ptemes' => null, 
									  	'ptesemestre' => null, 
									  	'pteano' => null, 
									  	'ptedtenvio' => null, 
									  	'pteobs' => null, 
									  	'ptedtvalidabolsa' => null, 
									  	'ptrcpfvalidabolsa' => null, 
									  );
    
    public function salvaExecucaoPT($post = null, $files = null)
    {
    	$arDados['pteid'] 	= $post['pteid'];
    	$arDados['ptrid'] 	= $post['ptrid'];
    	$arDados['pteobs'] 	= $post['pteobs'];
    
    	if($post['enviar_execucao']=='true'){
    		$arDados['ptedtenvio'] = date('Y-m-d H:i:s');
    	}
    
    	$this->popularDadosObjeto($arDados);
    	$pteid = $this->salvar();
    	$this->commit();
    	 
    	include_once APPRAIZ.'maismedicos/classes/Planotrabalho_VisitaProgramada.class.inc';
    	$obVp = new Planotrabalho_VisitaProgramada();
    	//echo ' <pre>'; var_dump($post); die;
    	$obVp->salvaVisitaProgramadaPT($post, $files);
    
    	include_once APPRAIZ.'maismedicos/classes/Planotrabalho_AtividadeAcompanhamento.class.inc';
    	$obAt = new Planotrabalho_AtividadeAcompanhamento();
    	$ptrid = $arDados['ptrid'];
    	$obAt->salvaAtividadeAcompanhamentoPT($ptrid, $post, $files);
    
    }
    
    public function excluiPlano($pteid = null)
    {
    	if($ptrid){
    		$sql = "update {$this->stNomeTabela} set ptestatus = 'I' where pteid = {$pteid};";
    		$this->executar($sql);
    		$this->commit();
    		return true;
    	}
    	return false;
    }
    
    public function recuperaExecucaoPorPlano($ptrid = null, $enviado =false)
    {
    	if($ptrid){
    	$sql = "select
    	case when ptrdtenvio is null then
    	'
    	<img title=\"Editar\" src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" class=\"btnEditaPlano\" id=\"' || pte.pteid || '\" />
    	<img title=\"Excluir\" src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" class=\"btnExcluiPlano\" id=\"' || pte.pteid || '\" />
    	'
    	else
    	'
    	<img title=\"Visualizar\" src=\"../imagens/lupa_grafico.gif\" style=\"cursor:pointer;\" class=\"btnEditaPlano\" id=\"' || pte.pteid || '\" />
    	' end as acao,
    	case when ptrsemestre = 1 then '1º Semestre' when ptrsemestre = 2 then '2º Semestre' else ptrsemestre::varchar end || ' de ' || ptrano as semestre,
    	case when ptedtenvio is null then 'Não enviado' else to_char(ptedtenvio, 'DD/MM/YYYY') end as dtenvio,
    	(select count(*) from maismedicos.planotrabalho_visitaprogramada ptv where ptv.ptrid = ptr.ptrid and ptv.ptvstatus = 'A') as visitas,
    	(select count(*) from maismedicos.planotrabalho_atividadeacompanhamento pta where pta.ptrid = ptr.ptrid and pta.ptastatus = 'A') as atividades,
    	'<center><img src=\"../imagens/exclui_p.gif\" /></center>' as pago
    	from {$this->stNomeTabela} pte
    	join maismedicos.planotrabalho ptr on pte.ptrid = ptr.ptrid
    	where ptestatus = 'A'
    	and pte.ptrid = '{$ptrid}'
    	".($enviado ? 'and ptedtenvio is not null' : '')."
    	order by pteid desc";
    	 
    	return $this->carregar($sql);
    	}
    	return array();
    }
    
    public function recuperaExecucaoPorId($pteid = null)
    {
    	if($ptrid>0){
    		$arWhere[] = "pteid = {$pteid}";
    	}else{
    		$arWhere[] = "ptedtenvio is null";
    	}
    	$sql = "select * from {$this->stNomeTabela} pte
    			join maismedicos.planotrabalho ptr on pte.ptrid = ptr.ptrid
    			where ptestatus = 'A' 
    			and ptrcpf = '{$_SESSION['usucpf']}'
    			".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')." 
    			order by pteid desc";
    	
    	return $this->pegaLinha($sql);
    }
    
    public function abreExecucao($post = null, $id = null)
    {
    	 
    	if($id) $post['pteid'] = $id;
    	$plano = $this->verificaExecucaoAberta($post);
    	//echo '<pre>'; var_dump($plano);die;
    
    	if(!$plano){
    		$arDados['perid'] 	= $post['perid'];
    		$arDados['ptrid'] 	= $this->verificaPlanoParaExecucao($post);
	    	$arDados['ptemes'] 	= $post['permes'];
	    	$arDados['ptesemestre'] = $post['permes'] < 7 ? 1 : 2;
	    	$arDados['pteano'] 	= $post['perano'];
	    	$arDados['ptecpf'] 	= $_SESSION['usucpf'];
	    	$this->popularDadosObjeto($arDados);
	    	$id = $this->salvar();	    
	    	$this->commit();
    		$plano = $this->verificaExecucaoAberta(array('ptrid'=>$id));
	    	//echo '<pre>';var_dump($id);die;
    	}
    
    	return $plano;
    }
    
    
 	public function verificaPlanoParaExecucao($post = null)
    {    	
    	if($post['permes']){
    		$arWhere[] = "ptrmes = {$post['permes']}";
    	}
    	if($post['perano']){
    		$arWhere[] = "ptrano = {$post['perano']}";
    	}
    	$sql = "select ptrid from maismedicos.planotrabalho where ptrstatus = 'A' and ptrdtenvio is not null
    			and ptrcpf = '{$_SESSION['usucpf']}' 
    			".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')." order by ptrid desc";
    	return $this->pegaUm($sql);
    }
    
 	public function verificaExecucaoAberta($post = null)
    {    	
    	if($post['pteid']){
    		$arWhere[] = "p.ptrid = {$post['ptrid']}";
    	}else
    	if($post['perid']){
    		$arWhere[] = "p.perid = {$post['perid']}";
    	}
    	$sql = "select e.*, ptrcpf, ptrnome, ptrobs from {$this->stNomeTabela} e
    			left join maismedicos.planotrabalho p on p.ptrid = e.ptrid
    			where e.ptestatus = 'A' and e.ptedtenvio is null
    			and p.ptrcpf = '{$_SESSION['usucpf']}' 
    			".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')." order by pteano desc, ptemes desc";
    	//echo $sql;
    	return $this->pegaLinha($sql);
    }
    
}