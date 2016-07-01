<?php
	
class Pagamento_Periodo extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "maismedicos.pagamento_periodo";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "perid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'perid' => null, 
									  	'atpid' => null, 
									  	'permes' => null, 
									  	'perano' => null, 
									  	'perobs' => null, 
									  	'perdtini' => null, 
									  	'perdtfim' => null, 
									  	'perstatus' => null, 
									  );
    
    public function pegaPeriodoPlano($dados = null)
    {
    	if($dados['perid']){
    		$arWhere[] = "perid = {$dados['perid']}";
    	}
    	if(is_numeric($dados)){
    		$arWhere[] = "perid = {$dados['perid']}";
    	}
    	$sql = "select * from {$this->stNomeTabela} where perstatus = 'A' ".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '');
    	return $this->carregar($sql);
    	
    }
    
    public function montaCabecalhoPeriodo($dados = null)
    {
    	$rs = $this->pegaPeriodoPlano($dados);
    	$html = '';
    	if($rs){
    		if(count($rs)==1){
    			$cor = $rs[0]['perdtini']<=date('Y-m-d') && $rs[0]['perdtfim']>=date('Y-m-d') ? 'blue' : 'gray';
    			$html .= "<b><font color='{$cor}'>Período de Preenchimento: ".formata_data($rs[0]['perdtini']).($rs[0]['perdtfim'] ? ' até '.formata_data($rs[0]['perdtfim']) : '').'';
    			$html .= '<br/>Mês Referência: '.$rs[0]['permes'].'/'.$rs[0]['perano'].'';
    			$html .= '<br/>'.($rs[0]['permes']<7 ? '1' :'2').'º semestre de '.$rs[0]['perano'].'';
    			$html .= '</font>';
    		}if(count($rs)>1){
    			foreach($rs as $d){
    				$cor = $d['perdtini']>=date('Y-m-d') && $d['perdtfim']<=date('Y-m-d') ? 'blue' : 'gray';
    				$arrPer[] = "<font color='{$cor}'>Período de Preenchimento: ".formata_data($d['perdtini']).($d['perdtfim'] ? ' até '.formata_data($d['perdtfim']) : '').' <br/>Mês Referência: '.($d['permes'].'/'.$d['perano']).'</font>';
    			}
    			$html .= "".implode(', ', $arrPer);
    		}
    	}
    	echo $html;
    }
    
    public function getPeriodoAtividadeAbertoPorPerfil($arAtividade = array())
    { 
    	$arPerfil = arrayPerfil();
    	if(!$this->testa_superuser()){
    		$arWhere[] = "pflcod in (".implode(',',$arPerfil).") ";
    	}
    	if($arAtividade){
    		$arWhere[] = "p.atpid in (".implode(',',$arAtividade).") ";
    	}
    	
    	$sql = "select p.*, pflcod from {$this->stNomeTabela} p
    			
    			join maismedicos.pagamento_atividadeperiodo a on a.atpid = p.atpid
    				and atpstatus = 'A'
    			where perstatus = 'A'
    			and p.perid not in (select perid from maismedicos.planotrabalho pt where ptrstatus = 'A' and ptrcpf = '{$_SESSION['usucpf']}' and ptrdtenvio is not null)
    			and p.perid not in (select perid from maismedicos.planotrabalho_execucao pt where ptestatus = 'A' and ptecpf = '{$_SESSION['usucpf']}' and ptedtenvio is not null)
    			and perdtini<=to_date(to_char(now(), 'YYYY-MM-DD'), 'YYYY-MM-DD') and perdtfim>=to_date(to_char(now(), 'YYYY-MM-DD'), 'YYYY-MM-DD')
    			".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')."
    			order by perano desc, permes desc";
    	//ver($sql);
    	return $this->pegaLinha($sql);
    }
    
    public function abrePreenchimentoPlano()
    {
    	$check = $this->getPeriodoAtividadeAbertoPorPerfil(array(PRAZO_CAD_PT_AVAL_CAMEM, PRAZO_CAD_PT_COOR_CAMEM));
    	//echo '<pre>'; var_dump($check);die;
    	if($_REQUEST['ptrid']){
    		$arWhere[] = "per.perid = (select perid from maismedicos.planotrabalho where ptrid = {$_REQUEST['ptrid']})";
    	}else
    	if($check){ 
    		$arWhere[] = "per.perid = {$check['perid']}";
    	}else{
    		$arWhere[] = "per.perid is null";
    	}
    	
    	$sql = "select per.* from {$this->stNomeTabela} per
    			left join maismedicos.planotrabalho ptr on per.perid = ptr.perid
    				and ptrstatus = 'A'
    				and ptrcpf = '{$_SESSION['usucpf']}'
    			where perstatus = 'A'  
    			and ptrdtenvio is null   			
    			".($arWhere ? ' and '.implode(' and ', $arWhere) : '').'
    			order by perano desc, permes desc';
    	//echo '<pre>'; var_dump($sql);die;
    	$rs =  $this->pegaLinha($sql);
    	if($rs){
    		$ptrid = $this->veririficaPlanoEnviado($rs);
    		if($ptrid){
    			$rs['existe'] = $ptrid;
    		}    		
    	}
    	return $rs;
    }
    
    public function checkExecucaoPendente($arAtiv =null)
    {
    	$arPerfil = arrayPerfil();
    	if(!$this->testa_superuser()){
    		$arWhere[] = "pflcod in (".implode(',',$arPerfil).") ";
    	}
    	if(is_array($arAtiv)){
    		$arWhere[] = " per.atpid in (".implode(', ',$arAtiv).") ";
    	}
    	$sql = "select per.perid from {$this->stNomeTabela} per
    			join maismedicos.pagamento_atividadeperiodo ati on ati.atpid = per.atpid
    			join maismedicos.planotrabalho ptr on per.permes::integer = ptr.ptrmes::integer
    				and per.perano::integer = ptr.ptrano::integer    				
    			left join maismedicos.planotrabalho_execucao pte on pte.ptrid = ptr.ptrid
    				and pte.ptestatus = 'A'
    			where ptr.ptrstatus = 'A' 
    			and ptrcpf = '{$_SESSION['usucpf']}'
    			and per.perstatus = 'A'
    			and to_char(perdtini, 'YYYYMMDD')<=to_char(now(), 'YYYYMMDD')
    			and to_char(perdtfim, 'YYYYMMDD')>=to_char(now(), 'YYYYMMDD')
    			and ptrdtenvio is not null
    			and ptedtenvio is null
    			".($arWhere ? ' and '.implode(' and ', $arWhere) : '')."
    			order by per.perano desc, per.permes desc";
    	//ver($sql);
    	return $this->pegaUm($sql);
    }
    
    public function abrePreenchimentoExecucao($post = null)
    {
    	
    	if($post['ptrid']){
    		$arWhere[] = "per.perid = (select perid from maismedicos.planotrabalho where ptrid = {$_REQUEST['ptrid']})";
    	}else{
    		$pendente = $this->checkExecucaoPendente(array(PRAZO_PTEXEC_AVAL_CAMEM, PRAZO_PTEXEC_COOR_CAMEM));
    		if($pendente){
    			$arWhere[] = "per.perid = {$pendente}";
    		}else{
	    		$periodo = $this->getPeriodoAtividadeAbertoPorPerfil(array(PRAZO_PTEXEC_AVAL_CAMEM, PRAZO_PTEXEC_COOR_CAMEM));
		    	if($periodo){ 
		    		$arWhere[] = "per.perid = {$periodo['perid']}";
		    	}else{
		    		$arWhere[] = "per.perid is null";
		    	}
    		}
    	}
    	//ver($pendente, $periodo['perid']);
    	$sql = "select per.*, pte.pteid, pte.ptrid from {$this->stNomeTabela} per
    			left join maismedicos.planotrabalho_execucao pte on per.perid = pte.perid
    				and ptestatus = 'A'
    				and ptecpf = '{$_SESSION['usucpf']}'
    			 where perstatus = 'A' 
    			 and pte.ptedtenvio is null
    			".($arWhere ? ' and '.implode(' and ', $arWhere) : '')."
    			order by perano desc, permes desc";
    	//ver($sql, d);
    	$rs =  $this->pegaLinha($sql);
    	//echo '<pre>';var_dump($rs);die;
    	if($rs){
    		$ptrid = $this->veririficaPlanoEnviado($rs);
    		//ver($ptrid, d);
    		if($ptrid){
    			$pteid = $this->veririficaExecucaoEnviada($rs);
    			if($pteid){
    				$rs['existe'] = $pteid;
    			}
    		}else{
    			$rs['semplano'] = true;
    		}
    		   		
    	}
    	return $rs;
    }
    
    public function veririficaPlanoEnviado($periodo)
    {
    	$sql = "select ptrid from maismedicos.planotrabalho  where ptrstatus = 'A' and ptrdtenvio is not null
    	and ptrmes = '{$periodo['permes']}' and ptrano = '{$periodo['perano']}' and ptrcpf = '{$_SESSION['usucpf']}'";
    	return $this->pegaUm($sql);
    }
    
    public function veririficaExecucaoEnviada($periodo)
    {
    	$sql = "select pteid from maismedicos.planotrabalho_execucao  where ptestatus = 'A' and ptedtenvio is not null
    	and perid = {$periodo['perid']} and ptecpf = '{$_SESSION['usucpf']}'";
    	return $this->pegaUm($sql);
    }
    
    public function recuperaPeriodo($post = array())
    {
    	$sql = "select *, to_char(perdtini, 'DD/MM/YYYY') as dtini, to_char(perdtfim, 'DD/MM/YYYY') as dtfim from {$this->stNomeTabela} where perid = '{$post['perid']}'";
    	$rs = $this->pegaLinha($sql);
    	
    	echo json_encode($rs);
    	die;
    }
    
    public function salvaPeriodo($post = array())
    {
    	$arDados['perid'] = $post['perid'] ? $post['perid'] : null;
    	$arDados['atpid'] = $post['atpid'];
    	$arDados['permes'] = $post['permes'];
    	$arDados['perano'] = $post['perano'];
    	$arDados['perdtini'] = formata_data_sql($post['perdtini']);
    	$arDados['perdtfim'] = formata_data_sql($post['perdtfim']);
    	$arDados['perobs'] = $post['perobs'];
    	
    	$this->popularDadosObjeto($arDados);
    	$this->salvar();
    	$this->commit();
    	
    	return true;
    }
    
    public function verificaMesRefExiste($post = null)
    {
    	$mes = $post['permes'];
    	$ano = $post['perano'];
    	$atpid = $post['atpid'];
    	$perid = $post['perid'];
    	$sql = "select * from {$this->stNomeTabela} where atpid = {$atpid} and permes = '{$mes}' and perano = '{$ano}'".
    	($perid ? " and perid not in ({$perid})" : '');
    	//echo $sql;
    	if($this->pegaLinha($sql)){
    		echo '1';
    		return true;
    	}
    	echo '2';
    	return false;
    }
}