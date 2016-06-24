begin transaction;
--commit;
--rollback;

--
-- Cadastra o sistema, os perfis, os itens menu e suas associações. Cria também
-- o esquema e as tabelas básicas para o funcionamento do módulo.
--
-- Renê de Lima Barbosa
-- 10/12/2007
--

-- SCHEMA E TABELAS

--DROP SCHEMA IF EXISTS cte;
--CREATE SCHEMA cte AUTHORIZATION phpsimec;

-- SISTEMA

insert into seguranca.sistema (
	sisid, sisdsc, sisurl, sisabrev, sisdiretorio, sisarquivo, sisfinalidade, sisrelacionado, sispublico, sisstatus, sisexercicio, sismostra, sisemail, paginainicial
) values (
	14, 'Brasil Profissionalizado', 'http://', 'Brasil Profissionalizado', 'brasilpro', 'brasilpro', null, null, null, 'A', false, true, $_SESSION['email_sistema'], 'inicio&acao=A'
);

-- PERFILS E MENUS

delete from seguranca.perfilusuario where pflcod in ( select pflcod from seguranca.perfil where sisid = 14 );
delete from seguranca.perfilmenu where pflcod in ( select pflcod from seguranca.perfil where sisid = 14 );
delete from seguranca.estatistica where mnuid in ( select mnuid from seguranca.menu where sisid = 14 );
delete from seguranca.auditoria where mnuid in ( select mnuid from seguranca.menu where sisid = 14 );
delete from seguranca.perfil where sisid = 14;
delete from seguranca.menu where sisid = 14;

-- PERFIS

insert into seguranca.perfil
	select
		nextval( 'perfil_pflcod_seq'::regclass ) as pfcod,
		pfldsc,
		pfldatainicio,
		pfldatafim,
		pflstatus,
		pflresponsabilidade,
		pflsncumulativo,
		pflfinalidade,
		pflnivel,
		pfldescricao,
		14 as sisid,
		pflsuperuser
	from seguranca.perfil
	where
		sisid = 13 and
		pflstatus = 'A';

-- MENUS

insert into seguranca.menu
	select
		mnucod,
		mnucodpai,
		mnudsc,
		mnustatus,
		replace( mnulink, 'cte','brasilpro' ) as mnulink,
		mnutipo,
		mnustile,
		mnuhtml,
		mnusnsubmenu,
		mnutransacao,
		mnushow,
		abacod,
		mnuhelp,
		14 as sisid,
		nextval( 'menu_mnuid_seq'::regclass ) as mnuid,
		mnuidpai,
		mnuordem
	from seguranca.menu
	where
		sisid = 13 and
		mnustatus = 'A';

-- RELACIONA PERFIS COM MENUS

insert into seguranca.perfilmenu
	select
		p.pflcod,		
		'A' as pmnstatus,
		m.mnuid
	from seguranca.menu m
		inner join seguranca.perfil p on
			p.sisid = m.sisid
	where
		m.sisid = 14 and
		p.pflstatus = 'A' and
		( m.mnudsc, p.pfldsc ) in (
			select
				m2.mnudsc,
				p2.pfldsc
			from seguranca.menu m2
				inner join seguranca.perfil p2 on
					p2.sisid = m2.sisid
				inner join seguranca.perfilmenu pm2 on
					pm2.mnuid = m2.mnuid and
					pm2.pflcod = p2.pflcod
			where
				m2.sisid = 13 and
				p2.pflstatus = 'A' and
				pm2.pmnstatus = 'A'
		)
	group by
		m.mnuid,
		p.pflcod;

-- ATRIBUI PERMISSÃO DE ACESSO

insert into seguranca.perfilmenu ( pflcod, pmnstatus, mnuid )
	select
		(select pflcod from seguranca.perfil where sisid=14 and pflsuperuser='t' and pflstatus='A'),
		'A',
		mnuid
	from seguranca.menu
	where sisid=13;


insert into cte.instrumento ( itrdsc ) values ( 'Brasil Profissionalizado' );

insert into cte.instrumento ( itrdsc ) values ( 'Brasil Profissionalizado (Municípios)' );

-- ABAS

insert into seguranca.aba ( abadsc, sisid )
	select abadsc, 14 from seguranca.aba where sisid = 13;

select abadsc, sisid from seguranca.aba where sisid = 14;

insert into seguranca.aba_menu ( abacod, mnuid )
	select a2.abacod, m2.mnuid from (
		select a.abadsc, m.mnucod from seguranca.aba a
		inner join seguranca.aba_menu am on am.abacod = a.abacod
		inner join seguranca.menu m on m.mnuid = am.mnuid
		where a.sisid = 13
	) as s
	inner join seguranca.aba a2 on a2.abadsc = s.abadsc and a2.sisid = 14
	inner join seguranca.menu m2 on m2.mnucod = s.mnucod and m2.sisid = 14

update seguranca.menu m1 set abacod = (
	select a2.abacod
	from seguranca.menu m
	inner join seguranca.aba a on a.abacod = m.abacod
	inner join seguranca.aba a2 on a2.abadsc = a.abadsc and a2.sisid = 14
	where m.mnucod = m1.mnucod
) where m1.sisid = 14;