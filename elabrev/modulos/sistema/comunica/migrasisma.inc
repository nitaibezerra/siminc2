<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:migrasisma.inc
   Finalidade: permitir migrar os dados de usuários e respectivos perfis do sisma para o serp
   */
// importação de responsabilidades


/*
 serão migrados os seguintes usuários/perfis: coordenadores de ação, gerentes, gerentes-executivos, equipes de apoio, super-usuários, coordenadores da uma, uma.
  */
  
if ($_REQUEST['migrap'])
{
// migrando os super-usuários

    $sql="select u.usucpf from seguranca.usuario u inner join seguranca.usuario_sistema us on us.susstatus='A' and u.usucpf=us.usucpf and us.sisid=".$_SESSION['sisid']." inner join seguranca.perfilusuario pu on pu.usucpf=us.usucpf and pu.pflcod=6 where u.usustatus ='A' and u.usucpf not in (select ppu.usucpf from seguranca.perfilusuario ppu where ppu.pflcod=23 )";

	$RS = $db->record_set($sql);
	$nlinhas = $db->conta_linhas($RS);
	if ($nlinhas > 0)
	{
		for ($i=0; $i<=$nlinhas;$i++)
   		{
   			$res = $db->carrega_registro($RS,$i);
     		// a linha abaixo transforma em variáveis todos os campos do array
     		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
     		$sql="insert into seguranca.perfilusuario (pflcod,usucpf) values (23,'$usucpf')";
   	        $saida=$db->executar($sql);	
     		print 'Usuários migrados como Super-Usuários:  '.$usucpf.'<br>';
     		$sql="select usucpf from seguranca.usuario_sistema where usucpf='$usucpf' and sisid=".$_SESSION['sisid'];
     		if (! $db->pegaUm($sql))
     		{
     			$sql ="insert into seguranca.usuario_sistema (sisid,pflcod,susdataultacesso,usucpf) values (2,23,now(),'$usucpf')";
     			$saida=$db->executar($sql);
     		}     		
	    }
     }
     $db->commit();


     
// migrando os coordenadores da uma
    $sql="select u.usucpf from seguranca.usuario u inner join seguranca.usuario_sistema us on us.susstatus='A' and u.usucpf=us.usucpf and us.sisid=".$_SESSION['sisid']." inner join seguranca.perfilusuario pu on pu.usucpf=us.usucpf and pu.pflcod=4 where u.usustatus ='A' and u.usucpf not in (select ppu.usucpf from seguranca.perfilusuario ppu where ppu.pflcod=36)";
       
	$RS = $db->record_set($sql);
	$nlinhas = $db->conta_linhas($RS);
	if ($nlinhas > 0)
	{
		for ($i=0; $i<=$nlinhas;$i++)
   		{
   			$res = $db->carrega_registro($RS,$i);
     		// a linha abaixo transforma em variáveis todos os campos do array
     		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
     		//$sql="insert into seguranca.perfilusuario (pflcod,usucpf) values (36,'$usucpf')";
   	       // $saida=$db->executar($sql);	
     		print 'Usuários migrados como Coordenadores da UMA:  '.$usucpf.'<br>';
     		$sql="select usucpf from seguranca.usuario_sistema where usucpf='$usucpf' and sisid=".$_SESSION['sisid'];
     		if (! $db->pegaUm($sql))
     		{
     			$sql ="insert into seguranca.usuario_sistema (sisid,pflcod,susdataultacesso,usucpf) values (2,36,now(),'$usucpf')";
     			$saida=$db->executar($sql);
     		}
     		
	    }
     }
     $db->commit();

     // migrando a uma
    $sql="select u.usucpf from seguranca.usuario u inner join seguranca.usuario_sistema us on us.susstatus='A' and u.usucpf=us.usucpf and us.sisid=".$_SESSION['sisid']." inner join seguranca.perfilusuario pu on pu.usucpf=us.usucpf and pu.pflcod=18 where u.usustatus ='A' and u.usucpf not in (select ppu.usucpf from seguranca.perfilusuario ppu where ppu.pflcod=34)";
	$RS = $db->record_set($sql);
	$nlinhas = $db->conta_linhas($RS);
	if ($nlinhas > 0)
	{
		for ($i=0; $i<=$nlinhas;$i++)
   		{
   			$res = $db->carrega_registro($RS,$i);
     		// a linha abaixo transforma em variáveis todos os campos do array
     		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
     		$sql="insert into seguranca.perfilusuario (pflcod,usucpf) values (34,'$usucpf')";
   	        $saida=$db->executar($sql);	
     		print 'Usuários migrados como UMA:  '.$usucpf.'<br>';
     		$sql="select usucpf from seguranca.usuario_sistema where usucpf='$usucpf' and sisid=".$_SESSION['sisid'];
     		if (! $db->pegaUm($sql))
     		{
     			$sql ="insert into seguranca.usuario_sistema (sisid,pflcod,susdataultacesso,usucpf) values (2,34,now(),'$usucpf')";
     			$saida=$db->executar($sql);
     		}     		
     		     		
	    }
     }
     $db->commit();

     // migrando os gerentes
     // fase 1... migrando os perfis
     // fase 2... migrando as responsabilidades
     // iniciando fase 1
    $sql="select u.usucpf from seguranca.usuario u inner join seguranca.usuario_sistema us on us.susstatus='A' and u.usucpf=us.usucpf and us.sisid=".$_SESSION['sisid']." inner join seguranca.perfilusuario pu on pu.usucpf=us.usucpf and pu.pflcod=2 where u.usustatus ='A' and u.usucpf not in (select ppu.usucpf from seguranca.perfilusuario ppu where ppu.pflcod=37)";
	$RS = $db->record_set($sql);
	$nlinhas = $db->conta_linhas($RS);
	if ($nlinhas > 0)
	{
		for ($i=0; $i<=$nlinhas;$i++)
   		{
   			$res = $db->carrega_registro($RS,$i);
     		// a linha abaixo transforma em variáveis todos os campos do array
     		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
     		$sql="insert into seguranca.perfilusuario (pflcod,usucpf) values (37,'$usucpf')";
   	        $saida=$db->executar($sql);	
     		print 'Usuários migrados como Gerentes:  '.$usucpf.'<br>';
     		$sql="select usucpf from seguranca.usuario_sistema where usucpf='$usucpf' and sisid=".$_SESSION['sisid'];
     		if (! $db->pegaUm($sql))
     		{
     			$sql ="insert into seguranca.usuario_sistema (sisid,pflcod,susdataultacesso,usucpf) values (2,37,now(),'$usucpf')";
     			$saida=$db->executar($sql);
     		}     		
     		     		
	    }
     }

     
     // iniciando fase 2
     $sql="select u.usucpf from seguranca.usuario u inner join seguranca.usuario_sistema us on us.susstatus='A' and u.usucpf=us.usucpf and us.sisid=".$_SESSION['sisid']." inner join seguranca.perfilusuario pu on pu.usucpf=us.usucpf and pu.pflcod=2 where u.usustatus ='A' ";

     $RS = $db->record_set($sql);
	 $nlinhas = $db->conta_linhas($RS);
	 if ($nlinhas > 0)
	 {
		for ($i=0; $i<=$nlinhas;$i++)
   		{
   			$res = $db->carrega_registro($RS,$i);
     		// a linha abaixo transforma em variáveis todos os campos do array
     		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
     		// com o cpf vejo quais os programas que ele é responsável
     		$sql = "select ur.prgid from monitora.usuarioresponsabilidade ur where ur.rpustatus ='A' and ur.usucpf='$usucpf' and ur.prgid is not null";
     		$RSp = $db->record_set($sql);
	        $nlinhasp = $db->conta_linhas($RSp);
	        if ($nlinhasp > 0)
	        {
		        for ($ii=0; $ii<=$nlinhasp;$ii++)
   		        {
   		        	$res = $db->carrega_registro($RSp,$ii);
     		        // a linha abaixo transforma em variáveis todos os campos do array
     		        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
     		        // com o prgid faço uma busca na tabela de programa e acho o prgcod
     		        $sql = "select p.prgcod from monitora.programa p where p.prgid=$prgid and p.prgano='".$_REQUEST['anoorigem']."'";
     		        $prgcod = $db->pegaUm($sql);
     		        // com o prgcod descubro o prgid na base elabrev
     		        if ($prgcod)
     		        {
     		        	$sql = "select p.prgid from elabrev.ppaprograma_proposta p where p.prgcod='$prgcod' and p.prsano='".$_REQUEST['anoorigem']."'";
     		        	$prgid = $db->pegaUm($sql);
     		        	// com o prgid e o usucpf verifico se já existe carga na base do elabrev para o perfil de gerente (37)
     		        	if ($prgid)
     		        	{
    		        	    $sql= "select prgid from elabrev.usuarioresponsabilidade where prgid=$prgid and usucpf='$usucpf' and pflcod=37";
     		        	    if (! $db->pegaUm($sql))
     		        	    {
     		        		// então insere o registro
     		        		    $sql = "insert into elabrev.usuarioresponsabilidade (prgid,usucpf,pflcod) values ($prgid,'$usucpf',37)";
     		        		    $saida=$db->executar($sql);

     		        	    }
     		            }     		        	
     		        }
	                
   			
   		        }
   		    }
   		}
	 }

	 $db->commit();
    
/////////////////////////

     // migrando os coordenadores de ação
     // fase 1... migrando os perfis
     // fase 2... migrando as responsabilidades
     // iniciando fase 1
    $sql="select u.usucpf from seguranca.usuario u inner join seguranca.usuario_sistema us on us.susstatus='A' and u.usucpf=us.usucpf and us.sisid=".$_SESSION['sisid']." inner join seguranca.perfilusuario pu on pu.usucpf=us.usucpf and pu.pflcod=1 where u.usustatus ='A' and u.usucpf not in (select ppu.usucpf from seguranca.perfilusuario ppu where ppu.pflcod=38)";
	$RS = $db->record_set($sql);
	$nlinhas = $db->conta_linhas($RS);
	if ($nlinhas > 0)
	{
		for ($i=0; $i<=$nlinhas;$i++)
   		{
   			$res = $db->carrega_registro($RS,$i);
     		// a linha abaixo transforma em variáveis todos os campos do array
     		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
     		$sql="insert into seguranca.perfilusuario (pflcod,usucpf) values (38,'$usucpf')";
   	        $saida=$db->executar($sql);	
     		print 'Usuários migrados como Coordenadores de Ação: '.$usucpf.'<br>';
     		$sql="select usucpf from seguranca.usuario_sistema where usucpf='$usucpf' and sisid=".$_SESSION['sisid'];
     		if (! $db->pegaUm($sql))
     		{
     			$sql ="insert into seguranca.usuario_sistema (sisid,pflcod,susdataultacesso,usucpf) values (2,38,now(),'$usucpf')";
     			$saida=$db->executar($sql);
     		}      		
	    }
     }

     // iniciando fase 2
     $sql="select u.usucpf from seguranca.usuario u inner join seguranca.usuario_sistema us on us.susstatus='A' and u.usucpf=us.usucpf and us.sisid=".$_SESSION['sisid']." inner join seguranca.perfilusuario pu on pu.usucpf=us.usucpf and pu.pflcod=1 where u.usustatus ='A' ";

     $RS = $db->record_set($sql);
	 $nlinhas = $db->conta_linhas($RS);
	 if ($nlinhas > 0)
	 {
		for ($i=0; $i<=$nlinhas;$i++)
   		{
   			$res = $db->carrega_registro($RS,$i);
     		// a linha abaixo transforma em variáveis todos os campos do array
     		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
     		// com o cpf vejo quais as ações que ele é responsável
     		$sql = "select ur.acaid from monitora.usuarioresponsabilidade ur where ur.rpustatus ='A' and ur.usucpf='$usucpf' and ur.acaid is not null";
     		$RSp = $db->record_set($sql);
	        $nlinhasp = $db->conta_linhas($RSp);
	        if ($nlinhasp > 0)
	        {
		        for ($ii=0; $ii<=$nlinhasp;$ii++)
   		        {
   		        	$res = $db->carrega_registro($RSp,$ii);
     		        // a linha abaixo transforma em variáveis todos os campos do array
     		        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
     		        // com o acaid faço uma busca na tabela de ação e acho o acacod
     		        $sql = "select a.acacod from monitora.acao a where a.acaid=$acaid and a.prgano='".$_REQUEST['anoorigem']."'";

     		        $acacod = $db->pegaUm($sql);
     		        // com o acacod descubro o acaid na base elabrev
     		        if ($acacod)
     		        {
     		        	$sql = "select a.acaid from elabrev.ppaacao_proposta a where a.acacod='$acacod' and a.prsano='".$_REQUEST['anoorigem']."'";

     		        	$acaid = $db->pegaUm($sql);
     		        	// com o acaid e o usucpf verifico se já existe carga na base do elabrev para o perfil de coordenador (38)
     		        	if ($acaid)
     		        	{
    		        	   $sql= "select acaid from elabrev.usuarioresponsabilidade where acaid=$acaid and usucpf='$usucpf' and pflcod=38";	        	
     		        	   if (! $db->pegaUm($sql))
     		        	   {
     		        		// então insere o registro
     		        		$sql = "insert into elabrev.usuarioresponsabilidade (acaid,usucpf,pflcod) values ($acaid,'$usucpf',38)";
     		        		$saida=$db->executar($sql);

     		        	   }
     		            }
     		        	
     		        }
	                
   			
   		        }
   		    }
   		}
	 }

     	 $db->commit();
    

/////////////////////////////////////	 

     // migrando os gerentes executivos
     // fase 1... migrando os perfis
     // fase 2... migrando as responsabilidades
     // iniciando fase 1
    $sql="select u.usucpf from seguranca.usuario u inner join seguranca.usuario_sistema us on us.susstatus='A' and u.usucpf=us.usucpf and us.sisid=".$_SESSION['sisid']." inner join seguranca.perfilusuario pu on pu.usucpf=us.usucpf and pu.pflcod=3 where u.usustatus ='A' and u.usucpf not in (select ppu.usucpf from seguranca.perfilusuario ppu where ppu.pflcod=39)";

	$RS = $db->record_set($sql);
	$nlinhas = $db->conta_linhas($RS);
	if ($nlinhas > 0)
	{
		for ($i=0; $i<=$nlinhas;$i++)
   		{
   			$res = $db->carrega_registro($RS,$i);
     		// a linha abaixo transforma em variáveis todos os campos do array
     		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
     		$sql="insert into seguranca.perfilusuario (pflcod,usucpf) values (39,'$usucpf')";
   	        $saida=$db->executar($sql);	
     		print 'Usuários migrados como Gerentes Executivos: '.$usucpf.'<br>';
     		$sql="select usucpf from seguranca.usuario_sistema where usucpf='$usucpf' and sisid=".$_SESSION['sisid'];
     		if (! $db->pegaUm($sql))
     		{
     			$sql ="insert into seguranca.usuario_sistema (sisid,pflcod,susdataultacesso,usucpf) values (2,39,now(),'$usucpf')";
     			$saida=$db->executar($sql);
     		} 
	    }
     }

     // iniciando fase 2
     $sql="select u.usucpf from seguranca.usuario u inner join seguranca.usuario_sistema us on us.susstatus='A' and u.usucpf=us.usucpf and us.sisid=".$_SESSION['sisid']." inner join seguranca.perfilusuario pu on pu.usucpf=us.usucpf and pu.pflcod=3 where u.usustatus ='A' ";

     $RS = $db->record_set($sql);
	 $nlinhas = $db->conta_linhas($RS);
	 if ($nlinhas > 0)
	 {
		for ($i=0; $i<=$nlinhas;$i++)
   		{
   			$res = $db->carrega_registro($RS,$i);
     		// a linha abaixo transforma em variáveis todos os campos do array
     		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
     		// com o cpf vejo quais os programas que ele é responsável
     		$sql = "select ur.prgid from monitora.usuarioresponsabilidade ur where ur.rpustatus ='A' and ur.usucpf='$usucpf' and ur.prgid is not null";
     		$RSp = $db->record_set($sql);
	        $nlinhasp = $db->conta_linhas($RSp);
	        if ($nlinhasp > 0)
	        {
		        for ($ii=0; $ii<=$nlinhasp;$ii++)
   		        {
   		        	$res = $db->carrega_registro($RSp,$ii);
     		        // a linha abaixo transforma em variáveis todos os campos do array
     		        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
     		        // com o prgid faço uma busca na tabela de programa e acho o prgcod
     		        $sql = "select p.prgcod from monitora.programa p where p.prgid=$prgid and p.prgano='".$_REQUEST['anoorigem']."'";
     		        $prgcod = $db->pegaUm($sql);
     		        // com o prgcod descubro o prgid na base elabrev
     		        if ($prgcod)
     		        {
     		        	$sql = "select p.prgid from elabrev.ppaprograma_proposta p where p.prgcod='$prgcod' and p.prsano='".$_REQUEST['anoorigem']."'";
     		        	$prgid = $db->pegaUm($sql);
     		        	// com o prgid e o usucpf verifico se já existe carga na base do elabrev para o perfil de gerente executivo (39)
     		        	if ($prgid)
     		        	{
    		        	    $sql= "select prgid from elabrev.usuarioresponsabilidade where prgid=$prgid and usucpf='$usucpf' and pflcod=39";
     		        	    if (! $db->pegaUm($sql))
     		        	    {
     		        		// então insere o registro
     		        		    $sql = "insert into elabrev.usuarioresponsabilidade (prgid,usucpf,pflcod) values ($prgid,'$usucpf',37)";
     		        		    $saida=$db->executar($sql);

     		        	    }
     		            }     		        	
     		        }
	                
   			
   		        }
   		    }
   		}
	 }
/////////////////////////////////////	 
    $db->commit(); 
         

/////////////////////////////////////	 

     // migrando a equipe de apoio a gerente
     // fase 1... migrando os perfis
     // fase 2... migrando as responsabilidades
     // iniciando fase 1
    $sql="select u.usucpf from seguranca.usuario u inner join seguranca.usuario_sistema us on us.susstatus='A' and u.usucpf=us.usucpf and us.sisid=".$_SESSION['sisid']." inner join seguranca.perfilusuario pu on pu.usucpf=us.usucpf and pu.pflcod=22 where u.usustatus ='A' and u.usucpf not in (select ppu.usucpf from seguranca.perfilusuario ppu where ppu.pflcod=40)";

	$RS = $db->record_set($sql);
	$nlinhas = $db->conta_linhas($RS);
	if ($nlinhas > 0)
	{
		for ($i=0; $i<=$nlinhas;$i++)
   		{
   			$res = $db->carrega_registro($RS,$i);
     		// a linha abaixo transforma em variáveis todos os campos do array
     		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
     		$sql="insert into seguranca.perfilusuario (pflcod,usucpf) values (40,'$usucpf')";
   	        $saida=$db->executar($sql);	
     		print 'Usuários migrados como Gerentes Executivos: '.$usucpf.'<br>';
     		$sql="select usucpf from seguranca.usuario_sistema where usucpf='$usucpf' and sisid=".$_SESSION['sisid'];
     		if (! $db->pegaUm($sql))
     		{
     			$sql ="insert into seguranca.usuario_sistema (sisid,pflcod,susdataultacesso,usucpf) values (2,40,now(),'$usucpf')";
     			$saida=$db->executar($sql);
     		} 
	    }
     }

     // iniciando fase 2
     $sql="select u.usucpf from seguranca.usuario u inner join seguranca.usuario_sistema us on us.susstatus='A' and u.usucpf=us.usucpf and us.sisid=".$_SESSION['sisid']." inner join seguranca.perfilusuario pu on pu.usucpf=us.usucpf and pu.pflcod=3 where u.usustatus ='A' ";

     $RS = $db->record_set($sql);
	 $nlinhas = $db->conta_linhas($RS);
	 if ($nlinhas > 0)
	 {
		for ($i=0; $i<=$nlinhas;$i++)
   		{
   			$res = $db->carrega_registro($RS,$i);
     		// a linha abaixo transforma em variáveis todos os campos do array
     		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
     		// com o cpf vejo quais os programas que ele é responsável
     		$sql = "select ur.prgid from monitora.usuarioresponsabilidade ur where ur.rpustatus ='A' and ur.usucpf='$usucpf' and ur.prgid is not null";
     		$RSp = $db->record_set($sql);
	        $nlinhasp = $db->conta_linhas($RSp);
	        if ($nlinhasp > 0)
	        {
		        for ($ii=0; $ii<=$nlinhasp;$ii++)
   		        {
   		        	$res = $db->carrega_registro($RSp,$ii);
     		        // a linha abaixo transforma em variáveis todos os campos do array
     		        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
     		        // com o prgid faço uma busca na tabela de programa e acho o prgcod
     		        $sql = "select p.prgcod from monitora.programa p where p.prgid=$prgid and p.prgano='".$_REQUEST['anoorigem']."'";
     		        $prgcod = $db->pegaUm($sql);
     		        // com o prgcod descubro o prgid na base elabrev
     		        if ($prgcod)
     		        {
     		        	$sql = "select p.prgid from elabrev.ppaprograma_proposta p where p.prgcod='$prgcod' and p.prsano='".$_REQUEST['anoorigem']."'";
     		        	$prgid = $db->pegaUm($sql);
     		        	// com o prgid e o usucpf verifico se já existe carga na base do elabrev para o perfil de equipe de apoio a gerente (40)
     		        	if ($prgid)
     		        	{
    		        	    $sql= "select prgid from elabrev.usuarioresponsabilidade where prgid=$prgid and usucpf='$usucpf' and pflcod=40";
     		        	    if (! $db->pegaUm($sql))
     		        	    {
     		        		// então insere o registro
     		        		    $sql = "insert into elabrev.usuarioresponsabilidade (prgid,usucpf,pflcod) values ($prgid,'$usucpf',407)";
     		        		    $saida=$db->executar($sql);

     		        	    }
     		            }     		        	
     		        }
	                
   			
   		        }
   		    }
   		}
	 }
/////////////////////////////////////	 
    $db->commit();   
      
/////////////////////////

     // migrando a equipe de apoio a coordenadores de ação
     // fase 1... migrando os perfis
     // fase 2... migrando as responsabilidades
     // iniciando fase 1
    $sql="select u.usucpf from seguranca.usuario u inner join seguranca.usuario_sistema us on us.susstatus='A' and u.usucpf=us.usucpf and us.sisid=".$_SESSION['sisid']." inner join seguranca.perfilusuario pu on pu.usucpf=us.usucpf and pu.pflcod=8 where u.usustatus ='A' and u.usucpf not in (select ppu.usucpf from seguranca.perfilusuario ppu where ppu.pflcod=41)";
	$RS = $db->record_set($sql);
	$nlinhas = $db->conta_linhas($RS);
	if ($nlinhas > 0)
	{
		for ($i=0; $i<=$nlinhas;$i++)
   		{
   			$res = $db->carrega_registro($RS,$i);
     		// a linha abaixo transforma em variáveis todos os campos do array
     		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
     		$sql="insert into seguranca.perfilusuario (pflcod,usucpf) values (41,'$usucpf')";
   	        $saida=$db->executar($sql);	
     		print 'Usuários migrados como Coordenadores de Ação: '.$usucpf.'<br>';
     		$sql="select usucpf from seguranca.usuario_sistema where usucpf='$usucpf' and sisid=".$_SESSION['sisid'];
     		if (! $db->pegaUm($sql))
     		{
     			$sql ="insert into seguranca.usuario_sistema (sisid,pflcod,susdataultacesso,usucpf) values (2,41,now(),'$usucpf')";
     			$saida=$db->executar($sql);
     		}      		
	    }
     }

     // iniciando fase 2
     $sql="select u.usucpf from seguranca.usuario u inner join seguranca.usuario_sistema us on us.susstatus='A' and u.usucpf=us.usucpf and us.sisid=".$_SESSION['sisid']." inner join seguranca.perfilusuario pu on pu.usucpf=us.usucpf and pu.pflcod=8 where u.usustatus ='A' ";

     $RS = $db->record_set($sql);
	 $nlinhas = $db->conta_linhas($RS);
	 if ($nlinhas > 0)
	 {
		for ($i=0; $i<=$nlinhas;$i++)
   		{
   			$res = $db->carrega_registro($RS,$i);
     		// a linha abaixo transforma em variáveis todos os campos do array
     		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
     		// com o cpf vejo quais as ações que ele é responsável
     		$sql = "select ur.acaid from monitora.usuarioresponsabilidade ur where ur.rpustatus ='A' and ur.usucpf='$usucpf' and ur.acaid is not null";
     		$RSp = $db->record_set($sql);
	        $nlinhasp = $db->conta_linhas($RSp);
	        if ($nlinhasp > 0)
	        {
		        for ($ii=0; $ii<=$nlinhasp;$ii++)
   		        {
   		        	$res = $db->carrega_registro($RSp,$ii);
     		        // a linha abaixo transforma em variáveis todos os campos do array
     		        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
     		        // com o acaid faço uma busca na tabela de ação e acho o acacod
     		        $sql = "select a.acacod from monitora.acao a where a.acaid=$acaid and a.prgano='".$_REQUEST['anoorigem']."'";

     		        $acacod = $db->pegaUm($sql);
     		        // com o acacod descubro o acaid na base elabrev
     		        if ($acacod)
     		        {
     		        	$sql = "select a.acaid from elabrev.ppaacao_proposta a where a.acacod='$acacod' and a.prsano='".$_REQUEST['anoorigem']."'";

     		        	$acaid = $db->pegaUm($sql);
     		        	// com o acaid e o usucpf verifico se já existe carga na base do elabrev para o perfil de coordenador (41)
     		        	if ($acaid)
     		        	{
    		        	   $sql= "select acaid from elabrev.usuarioresponsabilidade where acaid=$acaid and usucpf='$usucpf' and pflcod=41";	        	
     		        	   if (! $db->pegaUm($sql))
     		        	   {
     		        		// então insere o registro
     		        		$sql = "insert into elabrev.usuarioresponsabilidade (acaid,usucpf,pflcod) values ($acaid,'$usucpf',41)";
     		        		$saida=$db->executar($sql);

     		        	   }
     		            }
     		        	
     		        }
	                
   			
   		        }
   		    }
   		}
	 }

     	 $db->commit();    
    
    
}


?>