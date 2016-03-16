<?php
class Model_Seguranca_Usuario extends Simec_Db_Table
{
    protected $_schema  = 'seguranca';
    protected $_name    = 'usuario';
    protected $_primary = 'usucpf';
    
    public function getUsuarioOnline() 
    {
    	$select = $this->select();
    	$select->setIntegrityCheck(false);
    	$select->from(array('online' => 'seguranca.usuariosonline'), array("COALESCE(count(*),0) as online"));
    	
    	return $this->fetchRow($select);
    }
    
    public function getUsuarioByCPF($cpf)
    {
    	$select = $this->select();
    	$select->setIntegrityCheck(false);
    	$select->from(array('usuario' => 'seguranca.usuario'));
    	$select->where("usuario.usucpf = '{$cpf}'");
    	 
    	return $this->fetchRow($select);
    }
    
    public function getUsuarios() 
    {
    	$select = $this->select();
    	$select->setIntegrityCheck(false);
    	$select->from(array('usuario' => 'seguranca.usuario'), array("usuario.usucpf", "usuario.usunome"));
    	$select->joinInner(array('reponsabilidade' => 'demandas.usuarioresponsabilidade'), "usuario.usucpf = reponsabilidade.usucpf");
    	$select->joinInner(array('sistema' => 'seguranca.usuario_sistema'), "usuario.usucpf = sistema.usucpf");
    	$select->where("reponsabilidade.rpustatus = 'A'");
    	$select->where("sistema.susstatus = 'A'");
    	$select->where("sistema.suscod = 'A'");
    	$select->where("reponsabilidade.pflcod IN ('238')");
    	$select->where("reponsabilidade.celid = 2");
    	$select->order('usuario.usunome');
    	
    	return $this->fetchAll($select);
    }
    
    public function getPermissoes($cpf)
    {
    	$select = $this->select();
    	$select->setIntegrityCheck(false);
    	$select->from(array('resource' => 'seguranca.resource'), array('rscid', 'rscdsc', 'rscmodulo', 'rsccontroller', 'rscaction', 'rsctipo'));
    	$select->joinInner(array('sistema' => 'seguranca.sistema'), "sistema.sisid = resource.sisid", array('sisid', 'sisdiretorio', 'sisarquivo', 'sisdsc', 'sisurl', 'sisabrev', 'sisexercicio', 'paginainicial', 'sissnalertaajuda', 'sislayoutbootstrap'));
    	$select->joinInner(array('usuario_sistema' => 'seguranca.usuario_sistema'), "sistema.sisid = usuario_sistema.sisid", array('susdataultacesso'));
    	$select->joinInner(array('usuario' => 'seguranca.usuario'), "usuario.usucpf = usuario_sistema.usucpf", array());
    	$select->joinInner(array('perfilusuario' => 'seguranca.perfilusuario'), "usuario.usucpf = perfilusuario.usucpf", array());
    	$select->joinInner(array('perfil' => 'seguranca.perfil'), "perfilusuario.pflcod = perfil.pflcod AND perfil.sisid = sistema.sisid", array('pflnivel AS usunivel'));
    	$select->where("usuario_sistema.suscod = 'A'");
    	$select->where("usuario.usucpf = '{$cpf}'");
    	$select->where("usuario.suscod = 'A'");
    	$select->where("perfil.pflstatus = 'A'");
    	$select->where("sistema.sisstatus = 'A'");
    	
    	$select->group(array
    	(
    		'rscid', 'rscdsc', 'rscmodulo', 'rsccontroller', 'rscaction', 'rsctipo', 
    		'sistema.sisid', 'sisdiretorio', 'sisarquivo', 'sisdsc', 'sisurl', 'sisabrev', 'sisexercicio', 'paginainicial', 
    		'pflnivel', 'susdataultacesso', 'sissnalertaajuda', 'sislayoutbootstrap'
    	));
    	
    	$select->order('susdataultacesso DESC');
    	
		return $this->fetchAll($select);    		
    }
    
    public function getUltimoAcesso($cpf)
    {
    	$select = $this->select();
    	$select->setIntegrityCheck(false);
    	$select->from(array('sistema' => 'seguranca.sistema'), array('sisid', 'sisdiretorio', 'sisarquivo', 'sisdsc', 'sisurl', 'sisabrev', 'sisexercicio', 'paginainicial', 'sissnalertaajuda', 'sislayoutbootstrap', 'siszend'));
    	$select->joinInner(array('usuario_sistema' => 'seguranca.usuario_sistema'), "sistema.sisid = usuario_sistema.sisid", array('susdataultacesso'));
    	$select->joinInner(array('usuario' => 'seguranca.usuario'), "usuario.usucpf = usuario_sistema.usucpf", array());
    	$select->joinInner(array('perfilusuario' => 'seguranca.perfilusuario'), "usuario.usucpf = perfilusuario.usucpf", array());
    	$select->joinInner(array('perfil' => 'seguranca.perfil'), "perfilusuario.pflcod = perfil.pflcod AND perfil.sisid = sistema.sisid", array('pflnivel AS usunivel'));
    	$select->where("usuario_sistema.suscod = 'A'");
    	$select->where("usuario.usucpf = '{$cpf}'");
    	$select->where("usuario.suscod = 'A'");
    	$select->where("perfil.pflstatus = 'A'");
    	$select->where("sistema.sisstatus = 'A'");
    	 
    	$select->group(array
    	(
    		'sistema.sisid', 'sisdiretorio', 'sisarquivo', 'sisdsc', 'sisurl', 'sisabrev', 'siszend', 'sisexercicio', 'paginainicial',
    		'pflnivel', 'susdataultacesso', 'sissnalertaajuda', 'sislayoutbootstrap'
    	));
    	 
    	$select->order('susdataultacesso DESC');
    	
    	$select->limit(1);
    	 
    	return $this->fetchRow($select);
    }
}