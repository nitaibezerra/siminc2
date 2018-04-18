-- Excluindo campo de relação entre o acompanhamento e o Indicador PNC
ALTER TABLE acompanhamento.acompanhamento DROP COLUMN ipnid;

-- Criando campo da nova relação entre o acompanhamento e o Indicador PNC
ALTER TABLE acompanhamento.acompanhamento ADD COLUMN ipncod character(3);
COMMENT ON COLUMN acompanhamento.acompanhamento.ipncod IS 'Código do Indicador PNC';
-- Criando nova relação entre o acompanhamento e o Indicador PNC
ALTER TABLE acompanhamento.acompanhamento
  ADD CONSTRAINT fk_acompanhamento_ipncod FOREIGN KEY (ipncod)
      REFERENCES public.indicadorpnc (ipncod) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION;

-- Criando campo da nova relação entre o acompanhamento e UO
ALTER TABLE acompanhamento.acompanhamento ADD COLUMN unocod character(5);
COMMENT ON COLUMN acompanhamento.acompanhamento.unocod IS 'Código da UO';

-- Criando campo da nova relação entre o acompanhamento e Sub Unidade
ALTER TABLE acompanhamento.acompanhamento ADD COLUMN suocod character(6);
COMMENT ON COLUMN acompanhamento.acompanhamento.suocod IS 'Código da Subunidade';

-- SELECT * FROM acompanhamento.acompanhamento;
-- SELECT * FROM public.indicadorpnc
-- SELECT * FROM public.subunidadeorcamentaria
-- SELECT * FROM public.unidadeorcamentaria 
-- suocod character(6)
-- unocod character(5)