<?php

	ini_set("memory_limit","2000M");
	set_time_limit(0);

	$sql = "SELECT usunome, usuemail FROM seguranca.usuario WHERE usucpf IN ( '' )
				UNION ALL
			SELECT
	nome as usunome,
	entemail as usuemail
FROM 
	(

-- SECRETARIA MUNICIPAL
select DISTINCT ent.entnome Nome,
ent.entemail
from entidade.entidade ent
left join entidade.endereco ende           on ende.entid = ent.entid 
inner join territorios.municipio mun      on mun.muncod = ende.muncod
inner join (select max(ent.entid) enti, ende.muncod
                                                                                              from entidade.entidade ent
                                               INNER JOIN entidade.funcaoentidade                 fue ON fue.entid = ent.entid AND fue.funid in (7) AND fue.fuestatus = 'A'
                                               INNER JOIN entidade.funcao                    fun ON fun.funid = fue.funid
                                               INNER JOIN entidade.endereco              ende ON ende.entid = ent.entid 
                                   WHERE ent.entstatus = 'A' and ent.entemail is not null
                                   group by ende.muncod)  fea2 on  fea2.enti = ent.entid 
where ent.entstatus = 'A'
UNION ALL
-- SECRETARIA ESTADUAL
select DISTINCT ent.entnome Nome,
ent.entemail
from entidade.entidade ent
left join entidade.endereco ende           on ende.entid = ent.entid 
inner join territorios.municipio mun      on mun.muncod = ende.muncod
inner join (select max(ent.entid) enti, ende.muncod
                                                                                              from entidade.entidade ent
                                               INNER JOIN entidade.funcaoentidade                 fue ON fue.entid = ent.entid AND fue.funid in (6) AND fue.fuestatus = 'A'
                                               INNER JOIN entidade.funcao                    fun ON fun.funid = fue.funid
                                               INNER JOIN entidade.endereco              ende ON ende.entid = ent.entid 
                                   WHERE ent.entstatus = 'A' and ent.entemail is not null
                                   group by ende.muncod)  fea2 on  fea2.enti = ent.entid 
where ent.entstatus = 'A'

UNION ALL

-- SECRETARIO ESTADUAL
select DISTINCT ent.entnome Nome,
ent.entemail
from entidade.entidade ent
left join entidade.endereco ende           on ende.entid = ent.entid 
inner join territorios.municipio mun      on mun.muncod = ende.muncod
inner join (select max(ent.entid) enti, ende.muncod
                                                                                              from entidade.entidade ent
                                               INNER JOIN entidade.funcaoentidade                 fue ON fue.entid = ent.entid AND fue.funid in (25) AND fue.fuestatus = 'A'
                                               INNER JOIN entidade.funcao                    fun ON fun.funid = fue.funid
                                               INNER JOIN entidade.endereco              ende ON ende.entid = ent.entid 
                                   WHERE ent.entstatus = 'A' and ent.entemail is not null
                                   group by ende.muncod)  fea2 on  fea2.enti = ent.entid 
where ent.entstatus = 'A'


UNION ALL


--EQUIPE MUNICIPAL
SELECT DISTINCT  
				   		 	
--	usuario.usucpf, 
	usuario.usunome as nomeusuario,
	usuario.usuemail
FROM 		
	seguranca.perfil perfil 
inner join seguranca.perfilusuario perfilusuario    on perfil.pflcod = perfilusuario.pflcod and perfil.pflcod = 460
right join seguranca.usuario usuario		    	on usuario.usucpf = perfilusuario.usucpf
left join 
(
  select 
	unicod, 
	unidsc
  from 
	public.unidade
  where 
	unitpocod = 'U'
) as unidadex on usuario.unicod = unidadex.unicod				
	

left join  territorios.municipio municipio 	    	on municipio.muncod = usuario.muncod
inner join seguranca.usuario_sistema usuariosistema on usuario.usucpf = usuariosistema.usucpf
left join  entidade.entidade entidade		    	on usuario.entid = entidade.entid
left join  public.cargo cargo		    			on cargo.carid = usuario.carid
	
WHERE 
	usunome is not null  and usuariosistema.suscod = 'A' and usuariosistema.sisid = '23' and (perfil.pflcod = 460)  and perfil.pflnivel >= 1

UNION ALL

--EQUIPE ESTADUAL
SELECT DISTINCT  
				   		 	
--	usuario.usucpf, 
	usuario.usunome as nomeusuario,
	usuario.usuemail
FROM 		
	seguranca.perfil perfil 
inner join seguranca.perfilusuario perfilusuario    on perfil.pflcod = perfilusuario.pflcod and perfil.pflcod = 461
right join seguranca.usuario usuario		    	on usuario.usucpf = perfilusuario.usucpf
left join 
(
  select 
	unicod, 
	unidsc
  from 
	public.unidade
  where 
	unitpocod = 'U'
) as unidadex on usuario.unicod = unidadex.unicod				
	

left join  territorios.municipio municipio 	    	on municipio.muncod = usuario.muncod
inner join seguranca.usuario_sistema usuariosistema on usuario.usucpf = usuariosistema.usucpf
left join  entidade.entidade entidade		    	on usuario.entid = entidade.entid
left join  public.cargo cargo		    			on cargo.carid = usuario.carid
	
WHERE 
	usunome is not null  and usuariosistema.suscod = 'A' and usuariosistema.sisid = '23' and (perfil.pflcod = 461)  and perfil.pflnivel >= 1

) as foo
WHERE
	foo.entemail NOT IN ( SELECT usuemail FROM sispacto.emailenviado2 )
LIMIT 500
			";
	/*
	$sql = "SELECT
				usunome,
				usuemail
			FROM
				seguranca.usuario
			WHERE
				usucpf IN ('', '')";*/
	
	$arDadosUsuarios = $db->carregar( $sql );
//ver($arDadosUsuarios, d);
	$cc  = "";
	$cco = "";
	$i = 0;
	$sqlEmail = "";

	$assunto  = "Webconferência do Pacto com o Ministro da Educação";

	$conteudo = '<p>Aos Dirigentes de Educação.</p>

				<p align="justify">Convidamos os Dirigentes de Educação e suas equipes técnicas para assistirem à webconferência sobre o Pacto Nacional pela Alfabetização na Idade Certa, que será transmitida no dia 13 de novembro, às 10h, no endereço eletrônico <a href="http://portal.mec.gov.br/seb/transmissao">http://portal.mec.gov.br/seb/transmissao</a>. Na ocasião, o Exmo. Sr. Ministro da Educação, Aloízio Mercadante, fará um pronunciamento sobre o Pacto. Sugestões e dúvidas podem ser enviadas para o e-mail '.$_SESSION['email_sistema']. '. Contamos com a participação de todos.</p>

				Secretaria de Educação Básica<br>
				Ministério da Educação';
	
	foreach($arDadosUsuarios as $dados){
		$i++;
		enviar_email(array('nome'=>'Pacto Nacional pela Alfabetização na Idade Certa', 'email'=> $_SESSION['email_sistema']), $dados['usuemail'], $assunto, $conteudo, $cc, $cco );
		$sqlEmail = "INSERT INTO sispacto.emailenviado2 (usunome, usuemail) VALUES ('teste','".$dados['usuemail']."')";
		$db->executar($sqlEmail);
		$db->commit();
		$sqlEmail = "";
	}
	echo "Foram enviados ".$i." e-mails";
?>